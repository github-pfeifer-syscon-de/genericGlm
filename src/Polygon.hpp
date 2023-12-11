/*
 * Copyright (C) 2021 rpf
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

#include <cstdlib>
#include <string>
#include <vector>
#include <list>
#include "GenericGlmCompat.hpp"
#include <memory>
#include <GL/glu.h>



#include "PositionDbl.hpp"

namespace p2t { // use namespace as somewhere windose has a Polygon defined

enum class Bezier {
    none,
    quad,
    cubic
};
    
struct Polygon;
typedef std::shared_ptr<Polygon> SharedPoly;
typedef std::list<SharedPoly> Polygons;

class Point;

struct Polygon {
    Positions positions;
    std::vector<SharedPos> bezier;
    Bezier bezierType;
    SharedPos last;
    std::vector<std::shared_ptr<Polygon>> holes;
    Rect bound;
    Bound outer;
    std::vector<std::shared_ptr<Point>> posTemp;
    Polygon();


    std::vector<Point*> toTess();
    void nested(Polygons &outline);
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

}   // namespace p2t
