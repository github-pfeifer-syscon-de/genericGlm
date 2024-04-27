/*
 * Copyright (C) 2018 rpf
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


#include <vector>
#include <list>

//#include "active_ptr.hpp"
#include "Geom2.hpp"

//namespace psc {
//namespace gl {
//    class Geom2;
//    typedef psc::mem::active_ptr<Geom2> aptrGeom2;
//} /* end gl */
//} /* end psc */
//


class Displayable;

class GeometryContext
{
public:
    GeometryContext();
    virtual ~GeometryContext() = default;
    void addGeometry(const psc::gl::aptrGeom2& geo);
    void removeGeometry(Displayable *geo);
    int getPositionIndex();
    int getColorIndex();
    int getNormalIndex();
    int getUVIndex();
    int getNormMapTangentIndex();
    int getNormMapBitangentIndex();

    virtual bool useNormal();
    virtual bool useColor();
    virtual bool useUV();
    virtual bool useNormalMap();

    virtual void updateIndexes(unsigned int program);
    void setAllVisible(bool visible);
protected:
    int m_position_index;
    int m_color_index;
    int m_normal_index;
    int m_uv_index;
    int m_normMapTangentIndex;
    int m_normMapBitangentIndex;

    std::list<psc::gl::aptrGeom2> geom2;
private:
};
