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

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_IMAGE_H
#include FT_OUTLINE_H
#include FT_BBOX_H

typedef struct PositionDbl PositionDbl;
typedef std::shared_ptr<PositionDbl> SharedPos;
typedef std::vector<SharedPos> Positions;


struct PositionDbl
{
    GLdouble x;
    GLdouble y;
    GLdouble z;
    PositionDbl();
    PositionDbl(const PositionDbl& o) = default;
    PositionDbl(const FT_Vector& vect);
    PositionDbl(double _x, double _y, double _z);
    bool operator==(const PositionDbl& o);
    void min(PositionDbl &p);
    void max(PositionDbl &p);
    void middle(SharedPos p1, SharedPos p2);
};


struct Rect {
    PositionDbl p1,p2;

    Rect();
   /**
     * Computes the intersection of this <code>Rectangle</code> with the
     * specified <code>Rectangle</code>. Returns a new <code>Rectangle</code>
     * that represents the intersection of the two rectangles.
     * If the two rectangles do not intersect, the result will be
     * an empty rectangle.
     *
     * @param     r   the specified <code>Rectangle</code>
     * @return    the largest <code>Rectangle</code> contained in both the
     *            specified <code>Rectangle</code> and in
     *            this <code>Rectangle</code>; or if the rectangles
     *            do not intersect, an empty rectangle.
     */
    void intersection(Rect &r, Rect &res);
    bool empty() const;
    double area() const;
    std::string info() const;
};


// conversion from FT internal fix to floating point and back
GLdouble fixed2float(int x);
int float2fixed(float n);


struct Bound {
    PositionDbl xMin,xMax,yMin,yMax;
    static const size_t N;
    const PositionDbl get(int i) {
        switch (i) {
            case 0:
                return xMin;
            case 1:
                return xMax;
            case 2:
                return yMin;
            case 3:
                return yMax;
            default:
                return xMin;
        }
    }

    bool empty() const;
};
