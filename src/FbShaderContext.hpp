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

#include <memory>

#include "GenericGlmCompat.hpp"
#include "ShaderContext.hpp"
#include "Geom2.hpp"

class Framebuffer {
public:
    Framebuffer();
    virtual ~Framebuffer();

    void scale(GLuint width, GLuint height, GLint fbSampling, bool depth);
    void freeBuffer();
    void bind();
    void unbind();
    void bindTexture();
    GLuint getWidth();
    GLuint getHeight();
    GLint getSampling();
    GLuint getName();
private:
    GLuint m_framebufferId;
    GLuint m_textureId;
    GLuint m_depthbufId;
    GLuint atype, type, format;
    GLuint fbWidth, fbHeight;
    GLint fbSampling;

};

class FbShaderContext
: public ShaderContext {
public:
    FbShaderContext();

    virtual ~FbShaderContext() = default;
    void setup(unsigned int width, unsigned int height, unsigned int sampling);
    void updateLocation() override;
    void prepare();
    void done();
    void draw();

    Matrix setScalePos(const Matrix &m, Position &p, GLfloat scale) override;
    bool useColor() override;
    bool useNormal() override;
    bool useUV() override;
    bool createProgram(Glib::Error &error);
    void set_clear_color(const Color& clear_color);

private:
    //void updateIndexes(guint program);
    GLuint winWidth, winHeight;
    std::shared_ptr<Framebuffer> m_renderBuffer;
    // tried to improve antialiasing with separate buffer
    //   but using the first buffer with a larger size seems sufficient
    //std::shared_ptr<Framebuffer> m_resolveBuffer;
    psc::gl::aptrGeom2 m_box;

    GLuint m_texLocation;
    Color m_clear_color;
    //GLint m_uv_index;
};

