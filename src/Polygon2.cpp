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

#include <iostream>
#include <iomanip>
#include <fstream>


#include "Polygon2.hpp"

namespace psc {
namespace gl {

static const int BEZIER_POINTS = 2; // as we use a relative small display size this shoud be sufficient as 5 points lead to immense amount of vertexes e.g. > 400 for a 8 ...


Polygon2::Polygon2()
: positions()
, bezier()
, bezierType{Bezier::none}
, last()
{
}

Rect &
Polygon2::bounds()
{
    if (positions.empty()) {
        std::cerr << "Got polygon with no positions!" << std::endl;
    }
    else if(bound.empty()) {
        bound.p1 = *(positions.front().get());
        bound.p2 = *(positions.front().get());
        for (auto p : positions) {
            bound.p1.min(*(p.get()));
            bound.p2.max(*(p.get()));
        }
    }
    return bound;
}

Bound &
Polygon2::outers()
{
    if (positions.empty()) {
        std::cerr << "Got polygon with no positions!" << std::endl;
    }
    else if(outer.empty()) {
        SharedPos s = positions.front();
        outer.xMin = *s;
        outer.xMax = *s;
        outer.yMin = *s;
        outer.yMax = *s;
        for (auto p : positions) {
            if (p->x < outer.xMin.x) {
                outer.xMin = *p;
            }
            if (p->x > outer.xMax.x) {
                outer.xMax = *p;
            }
            if (p->y < outer.yMin.y) {
                outer.yMin = *p;
            }
            if (p->y > outer.yMax.y) {
                outer.yMax = *p;
            }
        }
    }
    return outer;
}


void
Polygon2::quadric_bezier(SharedPos p0, SharedPos p1,
        SharedPos p2, int n)
{
    double s = 1.0 / (double)n;
    double t = s;
    for (int i = 0; i < n; ++i) {
        double mt2 = (1.0f - t)*(1.0f - t);
        double tm2 = 2.0f * (1.0f - t) * t;
        double t2 = t * t;
        SharedPos n = std::make_shared<PositionDbl>();
        n->x = mt2 * p0->x + tm2 * p1->x + t2 * p2->x;
        n->y = mt2 * p0->y + tm2 * p1->y + t2 * p2->y;
        n->z = 0.0;
        positions.push_back(n);

        t += s;
    }
}

void
Polygon2::cubic_bezier(SharedPos p0, SharedPos p1, SharedPos p2, SharedPos p3, int n)
{
    double s = 1.0 / (double)n;
    double t = s;
    for (int i = 0; i < n; ++i) {
        double mt3 = (1.0f - t)*(1.0f - t)*(1.0f - t);
        double mt32 = 3.0f * (1.0f - t)*(1.0f - t) * t;
        double mt3_2 = 3.0f * (1.0f - t) * t * t;
        double t3 = t * t * t;
        SharedPos n = std::make_shared<PositionDbl>();
        n->x = mt3 * p0->x + mt32 * p1->x + mt3_2 * p2->x + t3 * p3->x;
        n->y = mt3 * p0->y + mt32 * p1->y + mt3_2 * p2->y + t3 * p3->y;
        n->z = 0.0;
        positions.push_back(n);

        t += s;
    }
}

void
Polygon2::addPoint(int tags, SharedPos pos)
{
    //std::cout << std::hex << "    0x" << (tags & 0x01) << " 0x" << (tags & 0x02)

    if ((tags & FT_CURVE_TAG_ON) != 0) {        // on point
        if (bezierType != Bezier::none) {       // the on-point after bezier will be "to" so create it
            if (bezierType == Bezier::cubic) {
                cubic_bezier(last, bezier[0], bezier[1], pos, BEZIER_POINTS);
            }
            else {  // is quad
                if (bezier.size() == 1) {
                    quadric_bezier(last, bezier[0], pos, BEZIER_POINTS);
                }
                else {      // create with a artifical midpoint
                    SharedPos mid = std::make_shared<PositionDbl>();
                    mid->middle(bezier[0], bezier[1]);
                    quadric_bezier(last, bezier[0], mid, BEZIER_POINTS);
                    quadric_bezier(mid, bezier[1], pos, BEZIER_POINTS);
                }
            }
            bezier.clear();
            bezierType = Bezier::none;
        }
        else {          // just normal on point
            positions.push_back(pos);
        }
        last = pos;
    }
    else {                      // off point -> bezier cntl
        if ((tags & FT_CURVE_TAG_CUBIC) == 0) { // 2. order
            bezierType = Bezier::quad;
        }
        else {                                  // 3. order
            bezierType = Bezier::cubic;
        }
        bezier.push_back(pos);  // collect bezier pnt
    }
    //if ((tags & 0x04) != 0) {
    //    int dropout = (tags >> 5) & 0x07;
    //    std::cout << "dropout " << dropout << std::endl;
    //}

}

// check https://www.freetype.org/freetype2/docs/glyphs/glyphs-6.html if we follow all rules
//  but latin glyphs look fine
void
Polygon2::addPoints(FT_Outline* outline, int start, int end, int cont)
{
    //std::cout << "  start " << start
    //          << " end " << end
    //          << std::endl;
    last = std::make_shared<PositionDbl>(outline->points[end]);  // if poly may start with conic but we might miss 2. part of paragraph
    for (int i = start; i <= end; ++i) {
        int tags = outline->tags[i];
        PositionDbl ppos(outline->points[i]);
        auto pos = std::make_shared<PositionDbl>(ppos);
        addPoint(tags, pos);
    }
    int tags = outline->tags[end];
    if (!(tags & FT_CURVE_TAG_ON)) { // close poly with start, if last was "off"
        auto pos = std::make_shared<PositionDbl>(outline->points[start]);
        tags = outline->tags[start];
        addPoint(tags, pos);
    }
}

void
Polygon2::clearTemp()
{
    for (auto n : holes) {
        n->clearTemp();
    }
    posTemp.clear();
}

std::vector<p2t::Point*>
Polygon2::toTess()
{
    std::vector<p2t::Point*> polyline;
#ifdef POLY2TRI
    for (auto p : positions) {
        bool insert = true;
        for (auto pt : polyline) {
            if (pt->x == p->x &&
                pt->y == p->y) {
                insert = false;
                break;
            }
        }
        if (insert) {
            // trick make shared so we are able to free afterwards
            auto pnt = std::make_shared<p2t::Point>(p->x, p->y);
            posTemp.push_back(pnt);
            polyline.push_back(pnt.get());
        }
    }
#endif
    return polyline;
}

// =============================================================================
// http://geomalgorithms.com/a03-_inclusion.html
// Copyright 2000 softSurfer, 2012 Dan Sunday
// This code may be freely used and modified for any purpose
// providing that this copyright notice is included with it.
// SoftSurfer makes no warranty for this code, and cannot be held
// liable for any real or imagined damage resulting from its use.
// Users of this code must verify correctness for their application.


// a Point is defined by its coordinates {int x, y;}
//===================================================================


// isLeft(): tests if a point is Left|On|Right of an infinite line.
//    Input:  three points P0, P1, and P2
//    Return: >0 for P2 left of the line through P0 and P1
//            =0 for P2  on the line
//            <0 for P2  right of the line
//    See: Algorithm 1 "Area of Triangles and Polygons"
double
Polygon2::isLeft(const SharedPos& P0,const SharedPos& P1,const PositionDbl& P2 )
{
    return ( (P1->x - P0->x) * (P2.y - P0->y)
            - (P2.x -  P0->x) * (P1->y - P0->y) );
}




// wn_PnPoly(): winding number test for a point in polygon
//      Input:   P = a point,
//      Return:  wn = the winding number (=0 only when P is outside)
int
Polygon2::windingNumber(const PositionDbl& P)
{
    int wn = 0;    // the  winding number counter
    //std::cout << "windingNumber for x " << P.x << " y " << P.y << std::endl;
    // loop through all edges of the polygon
    for (size_t i=0; i<positions.size(); i++) {   // edge from V[i] to  V[i+1]
        size_t i1 = (i+1) % positions.size();
        //std::cout << "  checking i " << i << " i1 " << i1 << " wn" << wn << std::endl;
        if (positions[i]->y <= P.y) {          // start y <= P.y
            if (positions[i1]->y  > P.y)      // an upward crossing
                 if (isLeft(positions[i], positions[i1], P) > 0.0)  // P left of  edge
                     ++wn;            // have  a valid up intersect
        }
        else {                        // start y > P.y (no test needed)
            if (positions[i1]->y  <= P.y)     // a downward crossing
                 if (isLeft(positions[i], positions[i1], P) < 0.0)  // P right of  edge
                     --wn;            // have  a valid down intersect
        }
    }
    //std::cout << "   wn " << wn << std::endl;
    return wn != 0 ? 1 : 0;
}
// =============================================================================


/*
 * find any nested in outline and assign to holes, remove from outline
 *   basic implementation working for latin letters with glu
 *    otherwise use winding number optional with a limit number of points
 */
void
Polygon2::nested(Polygons2 &outline)
{
    Rect rect = bounds();
    //std::cout << "nested " << rect.info()
    //          << std::endl;

    for (auto s = outline.begin(); s != outline.end(); ) {
        SharedPoly2 p = *s;
        Rect inner = p->bounds();
        //std::cout << "  bound " << inner.area() << std::endl;
        Rect res;
        rect.intersection(inner, res);
        bool separate = res.empty();
        //p->setPolygon(separate);
        if (!separate) {
            holes.push_back(p);
            //std::cout << "   assigning " << inner.info()
            //          << " res " << res.info()
            //          << std::endl;
            s = outline.erase(s);
        }
        else {
            ++s;
        }
        //rect = inner;
    }
}

void
Polygon2::gluTess(GLUtesselator *tess)
{
    gluTessBeginContour(tess);
    for (auto p : positions) {
        //std::cout << "Tess line " << i->x << "," << i->y << "," << i->z << std::endl;
        //GLdouble cords[3] = {i->x, i->y, i->z};
        gluTessVertex(tess, (GLdouble *)p.get(), (void *)p.get());
    }
    gluTessEndContour(tess);
    for (auto h : holes) {
        h->gluTess(tess);
    }
}

}   // namespace gl
}   // namespace psc
