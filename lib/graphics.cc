// -*- mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; -*-
// Copyright (C) 2014 Henner Zeller <h.zeller@acm.org>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation version 2.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://gnu.org/licenses/gpl-2.0.txt>

#include "graphics.h"
#include "utf8-internal.h"
#include <stdlib.h>
#include <functional>

namespace rgb_matrix {
int DrawText(Canvas *c, const Font &font,
             int x, int y, const Color &color,
             const char *utf8_text) {
  const int start_x = x;
  while (*utf8_text) {
    const uint32_t cp = utf8_next_codepoint(utf8_text);
    x += font.DrawGlyph(c, x, y, color, cp);
  }
  return x - start_x;
}
    
    
void DrawCircle(Canvas *c, int32_t x0, int32_t y0, int32_t radius, const Color &color){
int32_t x = radius, y = 0;
int32_t radiusError = 1 - x;
        
  while (y <= x){
    c->SetPixel(x + x0, y + y0, color.r, color.g, color.b);
    c->SetPixel(y + x0, x + y0, color.r, color.g, color.b);
    c->SetPixel(-x + x0, y + y0, color.r, color.g, color.b);
    c->SetPixel(-y + x0, x + y0, color.r, color.g, color.b);
    c->SetPixel(-x + x0, -y + y0, color.r, color.g, color.b);
    c->SetPixel(-y + x0, -x + y0, color.r, color.g, color.b);
    c->SetPixel(x + x0, -y + y0, color.r, color.g, color.b);
    c->SetPixel(y + x0, -x + y0, color.r, color.g, color.b);
    y++;
    if (radiusError<0){
      radiusError += 2 * y + 1;
    } else {
      x--;
      radiusError+= 2 * (y - x + 1);
    }
  }
}
    
void DrawLine(Canvas *c, int32_t x0, int32_t y0, int32_t x1, int32_t y1, const Color &color){
  int32_t dy = y1 - y0, dx = x1 - x0, gradient, x, y, shift = 0x10;
    
  if (abs(dx) > abs(dy)) {
    // x variation is bigger than y variation
    if (x1 < x0) {
      std::swap(x0, x1);
      std::swap(y0, y1);
    }
    gradient = (dy << shift) / dx ;
    
    for (x = x0 , y = 0x8000 + (y0 << shift); x <= x1; ++x, y += gradient) {
      c->SetPixel(x, y >> shift, color.r, color.g, color.b);
    }
  } else {
    // y variation is bigger than x variation
    if (y1 < y0) {
      std::swap(x0, x1);
      std::swap(y0, y1);
    }
    gradient = (dx << shift) / dy;
    for (y = y0 , x = 0x8000 + (x0 << shift); y <= y1; ++y, x += gradient) {
      c->SetPixel(x >> shift, y, color.r, color.g, color.b);
    }
  }
}

}//namespace
