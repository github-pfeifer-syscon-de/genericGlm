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

#include <glibmm.h>

#include "NaviContext.hpp"
#include "TextContext.hpp"

TextContext::TextContext(GLenum type)
: m_type{type}
, m_color_location{0}
, m_texture_location{0}
{
}


TextContext::~TextContext()
{
}

void TextContext::updateLocation()
{
    NaviContext::updateLocation();
    m_color_location = glGetUniformLocation(m_program, "color");
    if (m_type == GL_QUADS) {
        m_texture_location = glGetUniformLocation(m_program, "renderedTexture");
    }
}

void
TextContext::setColor(Color &c)
{
    glUniform3fv(m_color_location, 1, &c[0]);
}

bool
TextContext::useNormal() {
    return false;
}

bool
TextContext::useColor() {
    return false;
}

bool
TextContext::useUV() {
    return m_type == GL_QUADS;
}


void
TextContext::setTexture(int texture)
{
    glUniform1i(m_texture_location, texture);
}

bool
TextContext::createProgram(Glib::Error &error)
{
	#ifndef USE_GLES
	const char* version_frag = "130";
	const char* version_vert = "150 core";
	const char* precision = "";
	#else
    const char* version_frag = "300 es";
	const char* version_vert = "300 es";
	const char* precision = "precision mediump float;";
	#endif
	Glib::ustring vertex, fragment;
    if (m_type == GL_QUADS) {
        // Simplest texture shader
        vertex = Glib::ustring::sprintf(
            "#version %s\n"
            "in vec3 position;\n"
            "in vec2 uv;\n"
            "uniform mat4 mvp;	// perspective transform\n"
            "uniform vec3 color;\n"
            "out vec2 UV;\n"
            "out vec4 vertexColor;\n"
            "void main() {\n"
            "    vertexColor = vec4(color.rgb, 1.0);\n"
            "    gl_Position = mvp * vec4(position, 1.0);\n"
            "    UV = uv;\n"
            "}\n", version_frag);
        fragment = Glib::ustring::sprintf(
            "#version %s\n"
			"%s\n"
            "in vec2 UV;\n"
            "in vec4 vertexColor;\n"
            "out vec4 color;\n"
            "uniform sampler2D renderedTexture;\n"
            "void main() {\n"
            "    color = vertexColor * texture2D( renderedTexture, UV ).rgba;\n"
            "}\n", version_vert, precision);

    }
    else  {
        // Simplest triangle shader
        vertex = Glib::ustring::sprintf(
            "#version %s\n"
            "in vec3 position;\n"
            "uniform mat4 mvp;	// perspective transform\n"
            "uniform vec3 color;\n"
            "out vec4 vertexColor;\n"
            "void main() {\n"
            "    vertexColor = vec4(color.rgb, 1.0);\n"     // blend with backgr.?
            "    gl_Position = mvp * vec4(position, 1.0);\n"
            "}\n", version_frag);
        fragment = Glib::ustring::sprintf(
            "#version %s\n"
			"%s\n"
            "in vec4 vertexColor;\n"
            "out vec4 outputColor;\n"
            "void main() {\n"
            "  outputColor = vertexColor;\n"
            "}\n" , version_vert, precision);
    }
    bool ret = NaviContext::createProgram((gconstpointer)vertex.c_str(), (gconstpointer)fragment.c_str(), error);
    return ret;
}

psc::gl::aptrText2
TextContext::createText(NaviContext *ctx, const psc::gl::ptrFont2& font)
{
    auto text = psc::mem::make_active<psc::gl::Text2>(m_type, ctx, font);
    if (auto ltext = text.lease()) {
        ltext->setTextContext(this);
    }
    return text;
}