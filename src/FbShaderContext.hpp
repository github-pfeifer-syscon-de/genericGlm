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
#include "ShaderContext.hpp"

class FbShaderContext : public ShaderContext {
public:
    FbShaderContext();

    virtual ~FbShaderContext();
    void freeFramebuffer();
    void createFramebuffer();
    void setup(unsigned int width, unsigned int height, unsigned int sampling);
    void updateLocation() override;
    void prepare();
    void done();
    void draw();
    //void updateIndexes(guint program);
    GLuint width, height, atype, type, format;

    Matrix setScalePos(const Matrix &m, Position &p, GLfloat scale) override;
    bool useColor() override;
    bool useNormal() override;
    bool useUV() override;
    bool createProgram(Glib::Error &error);
    void set_clear_color(const Color& clear_color);

private:
    GLuint m_framebufferId;
    GLuint m_textureId;
    GLuint m_depthbufId;

    //GLuint quad_VertexArrayID;
    Geometry *m_box;

    GLuint m_texLocation;
    Color m_clear_color;
    //GLint m_uv_index;
};

