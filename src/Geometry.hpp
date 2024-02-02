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
#include <gdkmm.h>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <list>
#include <string>
#include <StringUtils.hpp>

#include "GeometryContext.hpp"
#include "Displayable.hpp"
void checkError(const char *where);

// as these primitives are required for geometry define them here




typedef struct vertex_info vertex_info;

/* position, color and normal information for each vertex */
struct vertex_info
{
    vertex_info()
    {
    }
    vertex_info(const Position &p, const Color &c, const Vector &n, const UV &u)
    : position(p)
    , color(c)
    , normal(n)
    , uv(u)
    , tangent(Vector(0.0f, 0.0f, 0.0f))
    , bitangent(Vector(0.0f, 0.0f, 0.0f))
    {
    }
    vertex_info(const Position &p, const Color &c, const Vector &n, const UV &u, const Vector &tangent, const Vector &bitangent)
    : position(p)
    , color(c)
    , normal(n)
    , uv(u)
    , tangent(tangent)
    , bitangent(bitangent)
    {
    }
    Position position;
    Color color;
    Vector normal;
    UV uv;
    Vector tangent;
    Vector bitangent;
};

typedef std::vector<GLfloat> Vertexts;
typedef std::vector<GLushort> Indexes;
typedef class Geometry Geometry;

class Geometry
: public Displayable
{
public:
    Geometry(GLenum type, GeometryContext *ctx);
    virtual ~Geometry();

    void addVertex(vertex_info &inf);
    void addRect(Position &p1, Position &p2, Color &c);
    void addTri(Position &p1, Position &p2, Position &p3, Color &c);
    void addLine(Position &p1, Position &p2, Color &c, Vector *n = nullptr);
    void addPoint(const Position *p, const Color *c = nullptr, const Vector *n = nullptr, const UV *uv = nullptr, const Vector *tangent = nullptr, const Vector *bitagent = nullptr);
    void addCube(float size, Color &c);
    void addSphere(float radius, unsigned int rings, unsigned int sectors);
    void addCylinder(float radius, float start, float end, Color *c, unsigned int approx = 36);
    unsigned int getVertexIndex();  // valid during creation

    void create_vao(); // send created vertexes to GPU
    virtual void display(const Matrix &mvp);     // display vao
    unsigned int getNumVertex();   // number of vertex valid after create_vao
    unsigned int getNumIndex();    //
    unsigned int getStrideSize();
    void addIndex(int idx);
    void addIndex(int idx1, int idx2);
    void addIndex(int idx1, int idx2, int idx3);
    Matrix getConcatTransform();
    void updateClickBounds(glm::mat4 &mvp);
    bool hit(float x, float y) override;
    Position &getViewMin() override;
    Position &getModelMin();
    Position &getModelMax();
    static void min(Position &set, const Position &other);
    static void max(Position &set, const Position &other);
    void addGeometry(Geometry *geo);
    virtual void setMaster(Geometry *geo);
    void removeGeometry(Geometry *geo);
    std::list<Displayable *> &getGeometries();
    void setDebugGeometry(Geometry *geo);
    void resetMaster() override;
    void setRemoveChildren(bool removeChildren);
    bool isRemoveChildren();
    void remove();
    virtual void setVisible(bool visible) override;
    void setSensitivity(float sensitivity);
    void addDestructionListener(GeometryDestructionListener *listener);
    void removeDestructionListener(GeometryDestructionListener *listener);
    void deleteVertexArray();
protected:
    std::list<Displayable *> geometries;
    Geometry *m_master;
    GLenum m_type;
    Geometry *debugGeom;
    float m_sensitivity;
    Position m_min,m_max;
    Position v_min,v_max;
    std::list<GeometryDestructionListener *> destructionListeners;
    bool m_removeChildren;
private:
    unsigned int m_vao;
    Vertexts m_vertexes; // temporary vertex buffer during building
    Indexes m_indexes;
    unsigned int m_numVertex;       // number of elements
    unsigned int m_stride_size;
    unsigned int m_numIndex;
};

typedef class Font Font;
typedef class NaviContext NaviContext;


class Tex {
public:
    Tex();
    virtual ~Tex();

    void create(Glib::RefPtr<Gdk::Pixbuf> pix);
    void create(GLuint width, GLuint height, const void *data, GLuint type = GL_RGB);
    void use(GLuint textureUnit = GL_TEXTURE0);
    void unuse();
    GLuint getTex();
    virtual GLint getMagFilter();
    virtual GLint getMinFilter();
    virtual GLint getTexWrap();
    static Tex *fromResource(const std::string res_name);
    static Tex *fromFile(const std::string file_name);

protected:
    GLuint m_tex;
private:
};


