// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#include "MigrateDataFiles.hpp"
#include "Dialogs/Message.hpp"
#include "Language/Language.hpp"
#include "LocalPath.hpp"
#include "LogFile.hpp"
#include "Profile/Profile.hpp"
#include "Repository/FileRepository.hpp"
#include "Repository/FileType.hpp"
#include "Repository/Parser.hpp"
#include "io/FileLineReader.hpp"
#include "system/FileUtil.hpp"
#include "system/Path.hpp"

#include <vector>

/**
 * File types to migrate, in priority order.  Types listed first
 * claim ambiguous extensions (e.g. .cup goes to waypoints/ rather
 * than tasks/, .igc goes to logs/ rather than tasks/) for files
 * not found in the repository.
 * Types with specific filename patterns (CHECKLIST, FLARMDB) are
 * listed before AIRSPACE so their patterns are matched before the
 * generic *.txt glob.
 * Types with a nullptr default directory are omitted since their
 * files remain in the data root.
 */
static constexpr FileType migration_order[] = {
  FileType::CHECKLIST,
  FileType::FLARMDB,
  FileType::AIRSPACE,
  FileType::WAYPOINT,
  FileType::WAYPOINTDETAILS,
  FileType::MAP,
  FileType::FLARMNET,
  FileType::IGC,
  FileType::NMEA,
  FileType::RASP,
  FileType::XCI,
  FileType::TASK,
};

/**
 * Visitor that collects full paths and base names of matching files.
 */
class CollectingVisitor final : public File::Visitor {
  std::vector<std::pair<AllocatedPath, AllocatedPath>> &files;

public:
  explicit CollectingVisitor(
    std::vector<std::pair<AllocatedPath, AllocatedPath>> &_files) noexcept
    :files(_files) {}

  void Visit(Path path, Path filename) override {
    files.emplace_back(AllocatedPath(path), AllocatedPath(filename));
  }
};

/**
 * Iterate NUL-separated pattern list from GetFileTypePatterns().
 */
static const char *
NextPattern(const char *p) noexcept
{
  while (*p != '\0')
    ++p;
  return p + 1;
}

/**
 * Try to load the cached repository file for file type
 * disambiguation.  Returns an empty repository on failure.
 */
static FileRepository
LoadRepository() noexcept
{
  FileRepository repository;
  try {
    const auto path = LocalPath("repository");
    FileLineReaderA reader(path);
    ParseFileRepository(repository, reader);
  } catch (...) {
    /* repository file may not exist — that's fine */
  }
  return repository;
}

void
MigrateDataFiles() noexcept
{
  if (Profile::Exists(ProfileKeys::DataDirVersion))
    return;

  if (ShowMessageBox(
        "Organize data files into subdirectories?\n"
           "Files in XCSoarData will be sorted into "
           "folders (airspace, waypoints, maps, etc.).",
        "XCSoar", MB_YESNO | MB_ICONQUESTION) != IDYES) {
    /* user declined — mark as done so we don't ask again */
    Profile::Set(ProfileKeys::DataDirVersion, "1");
    Profile::Save();
    return;
  }

  const auto root = GetPrimaryDataPath();
  const auto repository = LoadRepository();
  bool any_moved = false;

  for (const auto type : migration_order) {
    const auto subdir = GetFileTypeDefaultDir(type);
    if (subdir == nullptr)
      continue;

    const auto dest_dir = LocalPath(subdir);

    /* iterate each glob pattern for this file type */
    for (const char *pattern = GetFileTypePatterns(type);
         *pattern != '\0';
         pattern = NextPattern(pattern)) {

      /* collect matches in root (non-recursive) */
      std::vector<std::pair<AllocatedPath, AllocatedPath>> matches;
      CollectingVisitor visitor(matches);
      Directory::VisitSpecificFiles(root, pattern, visitor, false);

      for (const auto &[old_path, filename] : matches) {
        /* if the repository knows this file under a different type,
           skip it here — it will be handled when we iterate that
           type's patterns instead */
        const auto *repo_file =
          repository.FindByName(filename.c_str());
        if (repo_file != nullptr && repo_file->type != type)
          continue;

        auto new_path =
          AllocatedPath::Build(dest_dir, filename);

        if (File::Exists(new_path)) {
          /* both copies exist — keep the newer one */
          const auto old_time = File::GetLastModification(old_path);
          const auto new_time = File::GetLastModification(new_path);
          if (old_time > new_time) {
            /* root copy is newer — replace subdir copy */
            File::Delete(new_path);
          } else {
            /* subdir copy is same age or newer — delete root copy */
            File::Delete(old_path);
            LogFormat("MigrateDataFiles: removed older duplicate %s",
                      old_path.c_str());
            Profile::MigrateFilePath(old_path, new_path);
            any_moved = true;
            continue;
          }
        }

        if (MakeLocalPathRecursively(subdir, 10) == nullptr) {
          LogFormat("MigrateDataFiles: cannot create %s",
                    subdir.c_str());
          continue;
        }

        if (File::Rename(old_path, new_path)) {
          LogFormat("MigrateDataFiles: %s -> %s",
                    old_path.c_str(), new_path.c_str());
          Profile::MigrateFilePath(old_path, new_path);
          any_moved = true;
        } else {
          LogFormat("MigrateDataFiles: failed to move %s",
                    old_path.c_str());
        }
      }
    }
  }

  if (any_moved)
    Profile::Save();

  /* mark migration as done regardless of whether files were moved,
     so we don't re-scan on every startup */
  Profile::Set(ProfileKeys::DataDirVersion, "1");
  Profile::Save();
}
