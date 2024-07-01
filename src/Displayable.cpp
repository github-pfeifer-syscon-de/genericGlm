/* -*- Mode: c++; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4; coding: utf-8; -*-  */
/*
 * Copyright (C) 2023 RPf <gpl3@pfeifer-syscon.de>
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

#include <glm/vec4.hpp> // vec4
#include <glm/ext.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>   // euler
#include <glm/gtx/euler_angles.hpp> // eulerAngleYXZ

#include "GeometryContext.hpp"
#include "Displayable.hpp"

Displayable::Displayable(GeometryContext* _ctx)
: m_transform{1.0f}
, m_pos{0.0f}
, m_scale{1.0f}
, m_rotate{0.0f,0.0f,0.0f}
, m_visible{true}
, m_markable{true}
, m_ctx{_ctx}
{
}

void
Displayable::setScalePos(float _x, float _y, float _z, float _scale)
{
    m_pos.x = _x;
    m_pos.y = _y;
    m_pos.z = _z;
    m_scale = _scale;
    updateTransform();
}

void
Displayable::setPosition(Position &pos)
{
    m_pos = pos;
    updateTransform();
}

Position&
Displayable::getPosition()
{
    return m_pos;
}

void
Displayable::setScale(float _scale)
{
    m_scale = _scale;
    updateTransform();
}

void
Displayable::updateTransform()
{
    //Position p = m_pos;
    //glm::vec3 s(m_scale);
    //glm::mat4 eulerMat = glm::eulerAngleYX(m_rotate.phiRadians(), m_rotate.thetaRadians());
    //m_transform = glm::scale(glm::translate(glm::mat4(1.0f), p), s) * eulerMat;

    glm::mat4 translation = glm::translate(glm::mat4(1.0f), getPos());
    glm::mat4 rotation = glm::eulerAngleYXZ(m_rotate.phiRadians(), m_rotate.thetaRadians(), m_rotate.psiRadians());
    glm::mat4 scaling = glm::scale(glm::mat4(1.0f), glm::vec3(m_scale));
    m_transform = translation * rotation * scaling;
}

void
Displayable::setRotation(const Rotational &rotate)
{
    m_rotate = rotate;
    updateTransform();
}

Rotational &
Displayable::getRotation()
{
    return m_rotate;
}

float
Displayable::getScale()
{
    return m_scale;
}

Position &
Displayable::getPos()
{
    return m_pos;
}

bool
Displayable::isMarkable()
{
    return m_markable;
}

void
Displayable::setMarkable(bool markable)
{
    m_markable = markable;
}

bool
Displayable::isVisible()
{
    return m_visible;
}

void
Displayable::setVisible(bool visible)
{
    m_visible = visible;
}

Matrix &
Displayable::getTransform()
{
    return m_transform;
}

void
Displayable::setTransform(Matrix &m)
{
    m_transform = m;
}

void
Displayable::setContext(GeometryContext *ctx)
{
    m_ctx = ctx;
}

