/*
 * Copyright (C) 2019 rpf
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

// fix compatibility issues linux / win
// fix select openGL implementation GL ES 3 or generic
#pragma once

#include "config.h"

#ifdef __WIN32__    // compat msys2
#include <windef.h>
#endif

// alternatives
// #include <numbers>
// std::numbers::pi;
// #include <glm/ext.hpp>
// glm::pi;
// glm::pi_over_2
// glm::quarter_pi;

#ifndef M_PI
#define M_PI G_PI
#endif

#ifndef M_PI_2
#define M_PI_2 G_PI_2
#endif

#ifndef M_PI_4
#define M_PI_4 G_PI_4
#endif

#if USE_GLES
#include <GLES3/gl3.h>
#define GLAPIENTRY
#else
#include "epoxy/gl.h"  /* use full OpenGL */
#endif

