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

#include <glibmm.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>


#include "PositionDbl.hpp"

PositionDbl::PositionDbl()
: x{0.0}
, y{0.0}
, z{0.0}
{
}

PositionDbl::PositionDbl(const FT_Vector& vect)
: x{fixed2float(vect.x)}
, y{fixed2float(vect.y)}
, z{0.0}
{
}

PositionDbl::PositionDbl(double _x, double _y, double _z)
: x{_x}
, y{_y}
, z{_z}
{
}

bool
PositionDbl::operator==(const PositionDbl& o) {
    bool ret = (x == o.x
        && y == o.y
        && z == o.z);
    //std::cout << "this x " << x << " y " << y << " z " << z
    //          << " pos x " << o.x << " y " << o.y << " z " << o.z
    //          << " diff x " << x - o.x << " y " << y - o.y
    //          << " ret " << ret
    //          << std::endl;
    return ret;
}


void
PositionDbl::min(PositionDbl &p)
{
    x = p.x < x ? p.x : x;
    y = p.y < y ? p.y : y;
    z = p.z < z ? p.z : z;
}

void
PositionDbl::max(PositionDbl &p)
{
    x = p.x > x ? p.x : x;
    y = p.y > y ? p.y : y;
    z = p.z > z ? p.z : z;
}

void
PositionDbl::middle(SharedPos p1, SharedPos p2)
{
    double dx = (p2->x - p1->x) / 2.0;
    double dy = (p2->y - p1->y) / 2.0;
    double dz = (p2->z - p1->z) / 2.0;
    x = p1->x + dx;
    y = p1->y + dy;
    z = p1->z + dz;
}

Rect::Rect()
: p1()
, p2()
{
}

void Rect::intersection(Rect &r, Rect &res)
{
    double tx1 = p1.x;
    double ty1 = p1.y;
    double rx1 = r.p1.x;
    double ry1 = r.p1.y;
    double tx2 = p2.x;
    double ty2 = p2.y;
    double rx2 = r.p2.x;
    double ry2 = r.p2.y;
    if (tx1 < rx1) tx1 = rx1;
    if (ty1 < ry1) ty1 = ry1;
    if (tx2 > rx2) tx2 = rx2;
    if (ty2 > ry2) ty2 = ry2;
    tx2 -= tx1;
    ty2 -= ty1;
    // tx2,ty2 will never overflow (they will never be
    // larger than the smallest of the two source w,h)
    res.p1.x = tx1;
    res.p1.y = ty1;
    res.p2.x = tx1 + tx2;
    res.p2.y = ty1 + ty2;
}

bool
Rect::empty() const
{
    return !(p2.x - p1.x > 0.0
         && p2.y - p1.y > 0.0); // intersection creates negativ results for an empty area
}

double
Rect::area() const
{
    double dx = std::abs(p2.x - p1.x);
    double dy = std::abs(p2.y - p1.y);
    return dx * dy;
}

std::string
Rect::info() const
{
    return Glib::ustring::sprintf( "y%.1lf y%.1lf w%.1lf h%.1lf", p1.x, p1.y, p2.x - p1.x, p2.y - p1.y);
}


GLdouble
fixed2float(int x)
{
    int full = x >> 6;      // fixed 2 float
    int frac = x & 0x3f;
    GLdouble r = full + ((GLdouble)frac / 64.0);
    return r;
}

int
float2fixed(float n)
{
    return (int)(n * 64.0f);
}


bool
Bound::empty() const
{
    return (xMin.x == 0.0
         && xMin.y == 0.0
         && xMax.x == 0.0
         && xMax.y == 0.0
         && yMin.x == 0.0
         && yMin.y == 0.0
         && yMax.x == 0.0
         && yMax.y == 0.0);
}

const size_t Bound::N = 4;
