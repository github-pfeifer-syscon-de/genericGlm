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


#include "MarkContext.hpp"

MarkContext::MarkContext()
: NaviContext()
{
}

MarkContext::~MarkContext()
{
}

bool
MarkContext::useNormal() {
    return false;
}

bool
MarkContext::useColor() {
    return true;
}

bool
MarkContext::useUV() {
    return false;
}

bool
MarkContext::createProgram(Glib::Error &error)
{
	#ifndef USE_GLES
	const char* version = "130";
	const char* precision = "";
	#else
    const char* version = "300 es";
	const char* precision = "precision mediump float;";
	#endif
     // simplest / default shader
	auto vertex = Glib::ustring::sprintf("#version %s\n"
		"in vec3 position;\n"
		"in vec3 color;\n"
		"uniform mat4 mvp;\n"
		"out vec4 fColor;\n"
		"void main(){\n"
        // glglobe city marks
        "    gl_PointSize = 2.0;\n"
		"    gl_Position = mvp * vec4(position, 1.0);\n"
		"    fColor = vec4(color, 1.0);\n"
		"}\n", version);
	auto fragment = Glib::ustring::sprintf("#version %s\n"
		"%s\n"
		"in vec4 fColor;\n"
		"out vec4 color;\n"
		"void main() {\n"
		"    color = fColor;\n"
		"}\n", version, precision);
    bool ret = NaviContext::createProgram((gconstpointer)vertex.c_str(), (gconstpointer)fragment.c_str(), error);
    return ret;
}