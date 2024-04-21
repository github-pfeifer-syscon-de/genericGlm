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

#include <list>

#include "ShaderContext.hpp"

typedef class MarkContext MarkContext;

class NaviContext
: public ShaderContext
{
public:
    NaviContext();
    virtual ~NaviContext();

    Matrix setScalePos(const Matrix &perspectiveView, Position &p, GLfloat scale) override;
    Matrix setModel(const Matrix &perspectiveView, Matrix &model);
    void setMvp(glm::mat4 &mvp);
    Geometry *createGeometry(GLenum type);
    void display(const Matrix &perspectiveView);
    void display(const Matrix &perspectiveView, std::list<Displayable *> &geometries);
    void display(const Matrix &perspectiveView, std::list<psc::gl::aptrGeom2> &geom2);

    Geometry *hit(float x, float y);
    psc::gl::aptrGeom2 hit2(float x, float y);
protected:
    virtual void updateLocation() override;
    Displayable *hit(float x, float y, std::list<Displayable *> &chldGeos);
    psc::gl::aptrGeom2 hit2(float x, float y, std::list<psc::gl::aptrGeom2>& list);

private:
    GLint m_mvp_location;
};

