/* -*- Mode: c++; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * Copyright (C) 2024 RPf <gpl3@pfeifer-syscon.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <memory>
#include <list>

#include "PositionDbl.hpp"

namespace p2t {
class Point;
}

namespace psc {
namespace gl {


enum class Bezier {
    none,
    quad,
    cubic
};

struct Polygon2;
typedef std::shared_ptr<Polygon2> SharedPoly2;
typedef std::list<SharedPoly2> Polygons2;


struct Polygon2 {
    Positions positions;
    std::vector<SharedPos> bezier;
    Bezier bezierType;
    SharedPos last;
    std::vector<SharedPoly2> holes;
    Rect bound;
    Bound outer;
    std::vector<std::shared_ptr<p2t::Point>> posTemp;
    Polygon2();


    std::vector<p2t::Point*> toTess();
    void nested(Polygons2 &outline);
    double isLeft(const SharedPos& P0, const SharedPos& P1, const PositionDbl& P2);
    int windingNumber(const PositionDbl& P);
    Bound& outers();
    void gluTess(GLUtesselator *tess);

    void addPoint(int tags, SharedPos pos);
    void addPoints(FT_Outline* outline, int start, int end, int cont);
    Rect& bounds();
    void quadric_bezier(SharedPos  p0, SharedPos p1, SharedPos p2, int n);
    void cubic_bezier(SharedPos p0, SharedPos p1, SharedPos p2, SharedPos p3, int n);
    void clearTemp();
};

}   // namespace gl
}   // namespace psc