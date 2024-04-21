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


#include <glib.h>
#include <iostream>
#include <math.h>
#include <cmath>
#include <glm/vec4.hpp> // vec4
#include <glm/ext.hpp>

#include "Geometry.hpp"
#include "Geom2.hpp"
#include "TextContext.hpp"
#include "GeometryContext.hpp"

GeometryContext::GeometryContext()
: m_position_index{0}
, m_color_index{0}
, m_normal_index{0}
, m_uv_index{0}
, m_normMapTangentIndex{0}
, m_normMapBitangentIndex{0}
, m_text_context{nullptr}
 {
 }

GeometryContext::~GeometryContext()
{
    for (auto geo : geometries) {
        geo->setContext(nullptr);  // let any geometry forget about this context
                                //   as when destructing happens in a slightly illogic way
                                //     we dont want to try a remove that crashes
    }
    geometries.clear();
}

void
GeometryContext::addGeometry(Displayable *geo)
{
    geo->setRemoveFromCtx(true);
    geometries.push_back(geo);
}

void
GeometryContext::addGeometry(const psc::gl::aptrGeom2& geo)
{
    for (auto& geom : geom2) {
        if (geom == geo) {
            return;
        }
    }
    geom2.push_back(geo);
}

 void
 GeometryContext::removeGeometry(Displayable *geo)
 {
     geometries.remove(geo);
     for (auto iter = geom2.begin(); iter != geom2.end(); ) {
         auto& geo2 = *iter;
         if (!geo2 || geo2.get() == geo) {
             iter = geom2.erase(iter);
         }
         else {
             ++iter;
         }
     }
 }

 int
 GeometryContext::getPositionIndex()
 {
     return m_position_index;
 }

 int
 GeometryContext::getColorIndex()
 {
     return m_color_index;
 }

 int
 GeometryContext::getNormalIndex()
 {
     return m_normal_index;
 }

 int
 GeometryContext::getUVIndex()
 {
     return m_uv_index;
 }

int
GeometryContext::getNormMapTangentIndex()
{
    return m_normMapTangentIndex;
}

int
GeometryContext::getNormMapBitangentIndex()
{
    return m_normMapBitangentIndex;
}

 bool
 GeometryContext::useNormal() {
     return true;
 }

 bool
 GeometryContext::useColor() {
     return true;
 }

 bool
 GeometryContext::useUV() {
     return true;
 }

bool
GeometryContext::useNormalMap() {
    return false;
}

 void GeometryContext::updateIndexes(unsigned int program)
 {
     /* get the location of the "position" and "color" attributes */
     m_position_index = glGetAttribLocation(program, "position");
     if (m_position_index < 0)
     {
         std::cerr << "Error binding position" << std::endl;
     }
     if (useColor()) {
         m_color_index = glGetAttribLocation(program, "color");
         if (m_color_index < 0) {
             std::cerr << "Error binding color" << std::endl;
         }
     }
     if (useNormal()) {
         m_normal_index = glGetAttribLocation(program, "normal");
         if (m_normal_index < 0) {
             std::cerr << "Warn binding normal" << std::endl;
         }
     }
     if (useUV()) {
         m_uv_index =  glGetAttribLocation(program, "uv");
         if (m_uv_index < 0) {
             std::cerr << "Warn binding uv" << std::endl;
         }
     }
     if (useNormalMap()) {
         m_normMapTangentIndex = glGetAttribLocation(program, "normalMapTangent");
         if (m_normMapTangentIndex < 0) {
             std::cerr << "Warn binding normalMapTangent" << std::endl;
         }
         m_normMapBitangentIndex = glGetAttribLocation(program, "normalMapBitangent");
         if (m_normMapBitangentIndex < 0) {
             std::cerr << "Warn binding normalMapBitangent" << std::endl;
         }
     }
}

 void
 GeometryContext::setAllVisible(bool visible)
{
    for (auto& g : geometries) {
        g->setVisible(visible);
    }
    for (auto iter = geom2.begin(); iter != geom2.end(); ) {
        auto& geo = *iter;
        if (!geo) {
            iter = geom2.erase(iter);
        }
        else {
            if (auto lgeo = geo.lease()) {
                lgeo->setVisible(visible);
            }
            ++iter;
        }
    }
}

GeometryDestructionListener::GeometryDestructionListener()
{
}

GeometryDestructionListener::~GeometryDestructionListener()
{
}

