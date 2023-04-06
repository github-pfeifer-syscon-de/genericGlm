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

#include "GenericGlmCompat.hpp"
#include <glm/mat4x4.hpp> // glm::mat4

class Rotational {
public:
    Rotational(float phi = 0.0f, float theta = 180.0f, float psi = 0.0f);
    Rotational(const Rotational& orig);
    virtual ~Rotational();

    void add(float phi, float theta, float psi);
    void add(const Rotational &rot);
    float phiRadians();
    float thetaRadians();
    float psiRadians();
    static float deg2radians(float deg);
    float getPhi();
    float getTheta();
    float getPsi();
private:
    float  phi;
    float  theta;
    float  psi;
};

typedef glm::mat4 Matrix;

