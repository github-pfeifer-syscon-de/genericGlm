/* -*- Mode: c++; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * Copyright (C) 2024 RPf <gpl3@pfeifer-syscon.de>
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
#include <gdkmm.h>
#include <iostream>
#include <stdio.h>
#include <cmath>
#include <vector>
#include <list>
#include <StringUtils.hpp>

#include "active_ptr.hpp"
#include "Displayable.hpp"

class NaviContext;
class GeometryContext;

namespace psc {
namespace gl {


void checkError(const char *where);

// as these primitives are required for geometry define them here

class Geom2;

typedef psc::mem::active_ptr<Geom2> aptrGeom2;

typedef std::vector<GLfloat> Vertexts;
typedef std::vector<GLushort> Indexes;
typedef class Geometry Geometry;

class Geom2
: public Displayable
{
public:
    Geom2(GLenum type, GeometryContext *ctx);
    virtual ~Geom2();

    void setName(Glib::UStringView name);
    Glib::ustring& getName();
    void addRect(Position &p1, Position &p2, Color &c);
    void addTri(Position &p1, Position &p2, Position &p3, Color &c);
    void addLine(Position &p1, Position &p2, Color &c, Vector *n = nullptr);
    void addPoint(
            const Position* p
          , const Color* c = nullptr
          , const Vector* n = nullptr
          , const UV* uv = nullptr
          , const Vector* tangent = nullptr
          , const Vector* bitagent = nullptr);
    void addReserve(uint32_t addVertexes, uint32_t addIndex);
    void addCube(float size, Color& c);
    void addSphere(float radius, unsigned int rings, unsigned int sectors);
    void addCylinder(float radius, float start, float end, Color* c, unsigned int approx = 36);
    unsigned int getVertexIndex();  // valid during creation
    virtual void display(NaviContext* context, const Matrix &projView);

    void create_vao(); // send created vertexes to GPU
    virtual void display(const Matrix &mvp);     // display vao
    unsigned int getNumVertex();   // number of vertex valid after create_vao
    unsigned int getNumIndex();    //
    unsigned int getStrideSize();
    void addIndex(uint32_t idx);
    void addIndex(uint32_t idx1, uint32_t idx2);
    void addIndex(uint32_t idx1, uint32_t idx2, uint32_t idx3);
    Matrix getConcatTransform();
    void updateClickBounds(glm::mat4 &mvp);
    bool hit(float x, float y) override;
    Position &getViewMin() override;
    Position &getModelMin();
    Position &getModelMax();
    static void min(Position &set, const Position* pos);
    static void max(Position &set, const Position* pos);
    void addGeometry(aptrGeom2 geo);
    virtual void setMaster(Geom2* geo);
    void removeGeometry(Geom2* geo);
    std::list<aptrGeom2>& getGeom2();
    void setDebugGeometry(const aptrGeom2& geo);
    void resetMaster() override;
    virtual void setVisible(bool visible) override;
    void setSensitivity(float sensitivity);
    void deleteVertexArray();
protected:
    std::list<aptrGeom2> geometries;
    std::list<Displayable *> m_legacyGeometries;
    Geom2 *m_master;
    GLenum m_type;
    aptrGeom2 m_debugGeom;
    float m_sensitivity;
    Position m_min,m_max;
    Position v_min,v_max;
    Glib::ustring m_name;
private:
    unsigned int m_vao;
    Vertexts m_vertexes; // temporary vertex buffer during building
    Indexes m_indexes;
    unsigned int m_numVertex;       // number of elements
    unsigned int m_stride_size;
    unsigned int m_numIndex;

    static constexpr Color defCol{1.0f, 1.0f, 1.0f};
    static constexpr Vector defNor{0.0f, 1.0f, 0.0f};
    static constexpr UV defUV{0.0f, 0.0f};
    static constexpr Vector defTan{1.0f, 0.0f, 0.0f};
    static constexpr Vector defBitan{0.0f, 0.0f, 1.0f};
};

class Tex2;

typedef psc::mem::active_ptr<Tex2> aptrTex2;

class Tex2 {
public:
    Tex2();
    virtual ~Tex2();

    void create(Glib::RefPtr<Gdk::Pixbuf> pix);
    void create(GLuint width, GLuint height, const void *data, GLuint type = GL_RGB);
    void use(GLuint textureUnit = GL_TEXTURE0);
    void unuse();
    GLuint getTex();
    virtual GLint getMagFilter();
    virtual GLint getMinFilter();
    virtual GLint getTexWrap();
    static aptrTex2 fromResource(const std::string& res_name);
    static aptrTex2 fromFile(const std::string& file_name);

protected:
    GLuint m_tex;
private:
};





} /* namespace gl */
} /* namespace psc */