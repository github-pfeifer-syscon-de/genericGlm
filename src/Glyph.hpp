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

#include <glibmm.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <list>
#include <memory>
#include <GL/glu.h>

#include "ShaderContext.hpp"
#include "Geometry.hpp"
#include "PositionDbl.hpp"
#include "Polygon.hpp"



class Glyph
{
public:

    Glyph(gunichar _glyph, GeometryContext *geometryContext);
    ~Glyph();

    bool operator==(gunichar other) {
        return glyph == other;
    }
    GLfloat getAdvance() {
        return m_advance;
    }
    void displayLine(Matrix &mv) {
        if (m_lineGeom.getNumVertex() > 0) {     // allow glyphes without shape e.g. space
            m_lineGeom.display(mv);
        }
    }
    void display(Matrix &mv) {
        if (m_fillGeom.getNumVertex() > 0) {     // allow glyphes without shape e.g. space
            m_fillGeom.display(mv);
        }
    }
    GLint bindTexture() {
        if (m_tex > 0) {
            glBindTexture(GL_TEXTURE_2D, m_tex);
            checkError("TexShaderCtx glBindTextures");
        }
        return m_tex;
    }


    bool extractGlyph(FT_Library library, FT_Face face, FT_UInt glyph_index);
    void addFillPoint(const Position& pos, const Color &c, const Vector &n);
    GLuint getNumVertex();
    gunichar getChar() {
        return glyph;
    }
protected:
    gunichar glyph;
    GLfloat m_advance;
    GLfloat m_height;
    Geometry m_lineGeom;
    Geometry m_fillGeom;
    GLuint   m_tex;
    GeometryContext *m_ctx;

private:
    void render2tex(FT_Library library, FT_Face face, FT_UInt glyph_index);

    bool tesselate(Polygons lines);
    void setAdvance(GLfloat advance);
    void setHeight(GLfloat height);
    void addLine(std::shared_ptr<PositionDbl> pos, std::shared_ptr<PositionDbl> end, Color &c, Vector *n);
    void create_vao();
};
