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

#include "Geom2.hpp"
#include "NaviContext.hpp"
#include "MarkContext.hpp"

NaviContext::NaviContext()
: m_mvp_location{0}
{
}


NaviContext::~NaviContext()
{

}

void
NaviContext::updateLocation()
{
    m_mvp_location = glGetUniformLocation(m_program, "mvp");
    psc::gl::checkError("glGetUniformLocation ");
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
    psc::gl::checkError("glUniformMatrix4fv (mvp)");
}

psc::gl::aptrGeom2
NaviContext::createGeometry(GLenum type)
{
    auto geo = psc::mem::make_active<psc::gl::Geom2>(type, this);
    addGeometry(geo);
    return geo;
}

void
NaviContext::display(const Matrix &perspectiveView)
{
    display(perspectiveView, geom2);
    psc::gl::checkError("display geometries");
}

void
NaviContext::display(const Matrix &perspectiveView, std::list<psc::gl::aptrGeom2> &geom2)
{
    for (auto iter = geom2.begin(); iter != geom2.end(); ) {
        auto& geo = *iter;
        if (geo) {
            if (auto lgeo = geo.lease()) {
                glm::mat4 mvp = setModel(perspectiveView, lgeo->getTransform());
                lgeo->display(perspectiveView);
                lgeo->updateClickBounds(mvp);
                display(mvp, lgeo->getGeom2());
            }
            ++iter;
        }
        else {
            iter = geom2.erase(iter);
        }
    }
}

psc::gl::aptrGeom2
NaviContext::hit2(float x, float y)
{
    auto next_selected = hit2(x, y, geom2);
    return next_selected;
}


psc::gl::aptrGeom2
NaviContext::hit2(float x, float y, std::list<psc::gl::aptrGeom2>& list)
{
    psc::gl::aptrGeom2 next_selected;
    float lastViewdepth = 0.0f;
    for (auto iter = list.begin(); iter != list.end(); ) { // breadth first
        auto& geom2 = *iter;
        if (auto lgeo2 = geom2.lease()) {
            if (lgeo2->hit(x, y)) {
                if (!next_selected
                 || lastViewdepth > lgeo2->getViewMin().z) {
                    next_selected = geom2;
                    lastViewdepth = lgeo2->getViewMin().z;
                    // keep trying to find in same hierarchy a better match...
                }
            }
            ++iter;
        }
        else {
            iter = list.erase(iter);
        }
    }
    if (!next_selected) {  // unsure recuse even with selection to find better match ? (and what is a better match ???)
        for (auto& geo : list) {
            if (auto lgeo2 = geo.lease()) {
                auto& nextGeos = lgeo2->getGeom2();
                next_selected = hit2(x, y, nextGeos);
                if (next_selected) {
                    break;
                }
            }
        }
    }
    return next_selected;

}
