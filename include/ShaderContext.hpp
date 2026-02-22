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

#include <glibmm.h>

#include "GenericGlmCompat.hpp"
#include "GeometryContext.hpp"

#include "Matrix.hpp"
#include "Displayable.hpp"


class ShaderContext
: public GeometryContext
{
public:
    ShaderContext();
    virtual ~ShaderContext();

    GLuint getProgram() {
        return m_program;
    }
    void use() {
        if (m_program != 0)
            glUseProgram(m_program);
    }
    void unuse() {
        glUseProgram(0);
    }
    unsigned int createShader(int shader_type, gconstpointer source, Glib::Error &error);
    bool createProgram(unsigned int vertex, unsigned int fragment, unsigned int geometry, Glib::Error &error);
    bool createProgram(gconstpointer pVertex, gconstpointer pFrament, Glib::Error &error);

    virtual Matrix setScalePos(const Matrix &m, Position &p, GLfloat scale) = 0;
protected:
    virtual void updateLocation() = 0;
    GLuint m_program;
private:

};

void GLAPIENTRY
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam );

