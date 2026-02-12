// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#pragma once

/**
 * One-time migration of data files from XCSoarData root into
 * typed subdirectories (e.g. airspace/, waypoints/, maps/).
 *
 * Checks the profile key "DataDirVersion".  If absent, scans the
 * primary data path root for files matching known FileType patterns,
 * moves them into their designated subdirectory, updates profile
 * paths accordingly, and sets the version key so migration is not
 * repeated.
 */
void
MigrateDataFiles() noexcept;
