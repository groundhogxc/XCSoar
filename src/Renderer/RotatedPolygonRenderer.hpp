/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2016 The XCSoar Project
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

#ifndef XCSOAR_SCREEN_ROTATED_POLYGON_RENDERER_HPP
#define XCSOAR_SCREEN_ROTATED_POLYGON_RENDERER_HPP

#include "Screen/Canvas.hpp"
#include "Math/Angle.hpp"
#include "Util/Macros.hpp"

#include "Math/Screen.hpp"

#include <algorithm>

#include <assert.h>

class RotatedPolygonRenderer {
  BulkPixelPoint points[64];

public:
  RotatedPolygonRenderer(const BulkPixelPoint *src, unsigned n,
                         const PixelPoint pos, const Angle angle,
                         const unsigned scale=100)
  {
    assert(n <= ARRAY_SIZE(points));

    std::copy_n(src, n, points);
    PolygonRotateShift(points, n, pos, angle, scale);
  }

  void Draw(Canvas &canvas, unsigned start, unsigned n) const {
    assert(start + n <= ARRAY_SIZE(points));

    canvas.DrawPolygon(points + start, n);
  }
};

#endif
