/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2015 The XCSoar Project
  A detailed list of copyright holders can be found in the file "AUTHORS".

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
}
*/

#include "Screen/Bitmap.hpp"
#ifdef RENDER_OPENGL
#include "Screen/OpenGL/Texture.hpp"
#include "Screen/OpenGL/Surface.hpp"
#endif
#include "Android/NativeView.hpp"
#include "Android/Main.hpp"
#include "android_drawable.h"

Bitmap::Bitmap(ResourceId id)
#ifdef RENDER_OPENGL
  :texture(nullptr), interpolation(false)
#endif
{
  Load(id);
}

static const char *
find_resource_name(unsigned id)
{
  for (unsigned i = 0; DrawableNames[i].name != nullptr; ++i)
    if (DrawableNames[i].id == id)
      return DrawableNames[i].name;

  return nullptr;
}

#ifdef RENDER_OPENGL
gcc_malloc
static GLTexture *
LoadResourceTexture(ResourceId id, Bitmap::Type type)
{
  const char *name = find_resource_name((unsigned)id);
  if (name == nullptr)
    return nullptr;

  jint result[5];
  if (!native_view->loadResourceTexture(name, type == Bitmap::Type::MONO,
                                        result))
    return nullptr;

  return new GLTexture(result[0], result[1], result[2], result[3], result[4]);
}

gcc_malloc
static GLTexture *
LoadFileTexture(const TCHAR *path)
{
  jint result[5];
  if (!native_view->loadFileTexture(path, result))
    return nullptr;

  return new GLTexture(result[0], result[1], result[2], result[3], result[4]);
}
#endif

bool
Bitmap::Reload()
{
#ifdef RENDER_OPENGL
  assert(id.IsDefined() || !pathName.empty());
  assert(texture == nullptr);

  texture = id.IsDefined()
    ? LoadResourceTexture(id, type)
    : LoadFileTexture(pathName.c_str());
  if (texture == nullptr)
    return false;

  size.cx = texture->GetWidth();
  size.cy = texture->GetHeight();
#endif
  // TODO ANDROIDMEM: implement texture reload from Memory Bitmap

  return true;
}

bool
Bitmap::Load(ResourceId _id, Type _type)
{
  assert(_id.IsDefined());

  Reset();
#ifdef RENDER_OPENGL

  id = _id;
  type = _type;
  AddSurfaceListener(*this);

  if (!surface_valid)
    return true;
#endif
  // TODO ANDROIDMEM: implement texture reload from Memory Bitmap

  return Reload();
}

bool
Bitmap::LoadStretch(ResourceId id, unsigned zoom)
{
  assert(zoom > 0);

  // XXX
  return Load(id);
}

bool
Bitmap::LoadFile(const TCHAR *path)
{
  assert(path != nullptr && *path != _T('\0'));

  Reset();

#ifdef RENDER_OPENGL
  pathName = path;
  AddSurfaceListener(*this);

  if (!surface_valid)
    return true;

#endif
  // TODO ANDROIDMEM: implement texture reload from Memory Bitmap
  return Reload();
}

#ifdef RENDER_OPENGL
void
Bitmap::Reset()
{
  if (id.IsDefined() || !pathName.empty()) {
    RemoveSurfaceListener(*this);
    id = ResourceId::Null();
    pathName.clear();
  }

  delete texture;
  texture = nullptr;
}
#endif
  // TODO ANDROIDMEM: implement texture reload from Memory Bitmap

void
Bitmap::SurfaceCreated()
{
  Reload();
}

void
Bitmap::SurfaceDestroyed()
{
#ifdef RENDER_OPENGL
  delete texture;
  texture = nullptr;
#endif
  // TODO ANDROIDMEM: implement texture reload from Memory Bitmap
}
