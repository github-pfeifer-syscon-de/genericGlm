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

#include <glm/ext/matrix_transform.hpp>

#include "NaviContext.hpp"
#include "MarkContext.hpp"

NaviContext::NaviContext()
: m_mvp_location{0}
{
}


NaviContext::~NaviContext()
{
    // as references are need anyway to update most objects its left to
    //   the views to destruct geometries
    geometries.clear();
}

void
NaviContext::updateLocation()
{
    m_mvp_location = glGetUniformLocation(m_program, "mvp");
    checkError("glGetUniformLocation ");
}

Matrix
NaviContext::setScalePos(const Matrix &perspectiveView, Position &p, GLfloat scale)
{
    glm::vec3 s(scale);
    glm::vec3 pos(p/scale);   // the position works only with scale
    glm::mat4 model = glm::translate(glm::scale(glm::mat4(1.0f), s), pos);

    return setModel(perspectiveView, model);
}

Matrix
NaviContext::setModel(const Matrix &perspectiveView, Matrix &model)
{
    Matrix mvp = perspectiveView * model;
    setMvp(mvp);
    return mvp;
}

void
NaviContext::setMvp(glm::mat4 &mvp)
{
    glUniformMatrix4fv(m_mvp_location, 1, GL_FALSE, &mvp[0][0]);
    checkError("glUniformMatrix4fv (mvp)");
}

Geometry *
NaviContext::createGeometry(GLenum type)
{
    Geometry *geo = new Geometry(type, this);
    addGeometry(geo);
    return geo;
}

void
NaviContext::display(const Matrix &perspectiveView)
{
    display(perspectiveView, geometries);
    checkError("display geometries");
}

void
NaviContext::display(const Matrix &perspectiveView, std::list<Geometry *> &geometries)
{
    for (auto p = geometries.begin(); p != geometries.end(); ++p) {
        Geometry *g = *p;
        glm::mat4 mvp = setModel(perspectiveView, g->getTransform());
        g->display(perspectiveView);
        g->updateClickBounds(mvp);
        display(mvp, g->getGeometries());
    }
}

Geometry *
NaviContext::hit(float x, float y)
{
    Geometry *next_selected = hit(x, y, geometries);
    //if (next_selected != nullptr) {
    //    std::cout << "Selected " << typeid(*next_selected).name() << std::endl;
    //}
    return next_selected;
}

Geometry *
NaviContext::hit(float x, float y, std::list<Geometry *> &chldGeos)
{
    Geometry *next_selected = nullptr;
    for (auto p = chldGeos.begin(); p != chldGeos.end(); ++p) { // breadth first
        Geometry *g = *p;
        if (g->hit(x, y)) {
            if (next_selected == nullptr
             || next_selected->getViewMin().z > g->getViewMin().z) {
                next_selected = g;
                // keep trying to find in same hierarchy a better match...
            }
        }
    }
    if (next_selected == nullptr) {  // unsure recuse even with selection to find better match ? (and what is a better match ???)
        for (auto p = chldGeos.begin(); p != chldGeos.end(); ++p) {
            Geometry *g = *p;
            std::list<Geometry *> &nextGeos = g->getGeometries();
            next_selected = hit(x, y, nextGeos);
            if (next_selected != nullptr) {
                break;
            }
        }
    }
    return next_selected;
}

