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

#include <iostream>

#include <cmath>
#include <glm/trigonometric.hpp>    // glm::radians

#include "Matrix.hpp"

Rotational::Rotational(float _phi, float _theta, float _psi)
: phi{_phi}
, theta{_theta}       // look toward -z, to make right handed coord working
, psi{_psi}
{
}

void
Rotational::add(float _phi, float _theta, float _psi)
{
    phi += _phi;
    theta += _theta;
    psi += _psi;
}

void
Rotational::add(const Rotational &rot)
{
    phi += rot.phi;
    theta += rot.theta;
    psi += rot.psi;
}

float
Rotational::deg2radians(float deg)
{
    return glm::radians(deg);
}

float
Rotational::phiRadians()
{
    return glm::radians(phi);
}

float
Rotational::thetaRadians()
{
    return glm::radians(theta);
}

float
Rotational::psiRadians()
{
    return glm::radians(psi);
}

float
Rotational::getPhi()
{
    return phi;
}
float
Rotational::getTheta()
{
    return theta;
}
float
Rotational::getPsi()
{
    return psi;
}