/* -*- Mode: c++; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
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

#pragma once

#include <glm/vec2.hpp> // glm::vec2
#include <glm/vec3.hpp> // glm::vec3
#include <Matrix.hpp>


#include "GenericGlmCompat.hpp"

class GeometryContext;

typedef glm::vec3 Vector;

typedef glm::vec2 UV;

typedef glm::vec3 Position;

typedef glm::vec3 Color;

typedef glm::vec4 ColorAlpha;

class Displayable
{
public:
    Displayable(GeometryContext* _ctx);
    explicit Displayable(const Displayable& orig) = delete;
    virtual ~Displayable() = default;

    virtual void display(const Matrix &mvp) = 0;
    virtual void updateClickBounds(glm::mat4 &mvp) = 0;
    virtual void resetMaster() = 0;
    virtual bool hit(float x, float y) = 0;
    virtual Position& getViewMin() = 0;

    void setScalePos(float _x, float _y, float _z, float _scale);
    void setPosition(Position &pos);
    Position& getPosition();
    virtual void setScale(float _scale);
    void updateTransform();
    void setRotation(const Rotational &rotate);
    Rotational &getRotation();
    float getScale();
    Position &getPos();
    Matrix &getTransform();
    void setTransform(Matrix &m);
    void setMarkable(bool markable);
    bool isMarkable();
    bool isVisible();
    virtual void setVisible(bool visible);
    void setContext(GeometryContext* ctx);
protected:
    Matrix m_transform;
    Position m_pos;
    float m_scale;
    Rotational m_rotate;
    bool m_visible;
    bool m_markable;
    GeometryContext *m_ctx;

private:

};

