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

#include <stdlib.h>
#include <math.h>
#include <iostream>

#include "GenericGlmCompat.hpp"
#include "Diagram.hpp"

Diagram::Diagram(guint _size, NaviContext *_naviContext, TextContext *_textCtx)
: m_StripeWidth{0.3f}
, m_diagramHeight{0.7f}
, m_diagramWidth{2.0f}
, base{nullptr}
, m_values()
, m_font{nullptr}
, m_sname{nullptr}
, m_pmax{nullptr}
, m_smax{nullptr}
, naviContext{_naviContext}
, m_textCtx{_textCtx}
, m_size{_size}
{
}


Diagram::~Diagram()
{
    for (auto i : m_values) {
        Geometry *v = i;
        if (v != nullptr)
            delete v;
    }
    m_values.clear();
    if (m_sname)
        delete m_sname;
    if (m_pmax)
        delete m_pmax;
    if (m_smax)
        delete m_smax;
    if (base)
        delete base;
    base = nullptr;
}

gfloat
Diagram::index2x(guint i)
{
    return ((gfloat) i / (gfloat)(m_size-1)) * m_diagramWidth;
}

gfloat
Diagram::value2y(gfloat val)
{
    return val * m_diagramHeight;
}

void
Diagram::setPosition(Position &pos)
{
    getBase()->setPosition(pos);
}

void
Diagram::setFont(Font *font)
{
    m_font = font;
}

void
Diagram::setName(const Glib::ustring &sname) {
    if (m_sname == nullptr) {
        m_sname = m_textCtx->createText(naviContext, m_font);
        Position ptn(0.0f, m_diagramHeight - m_font->getLineHeight() * 0.007f, 0.0f);
        m_sname->setPosition(ptn);
        m_sname->setScale(0.010f);
        getBase()->addGeometry(m_sname);
    }
    m_sname->setText(sname);
}

Geometry *
Diagram::fill_data(const std::shared_ptr<Buffer<double>> &data, Gdk::RGBA *color, gfloat zOffs, gfloat yOffs)
{
    Geometry *geom = new Geometry(GL_TRIANGLES, naviContext);
    Color c((gfloat)color->get_red(), (gfloat)color->get_green(), (gfloat)color->get_blue());
    for (guint i = 0; i < m_size - 1; ++i) {
        Position p1(index2x(i    ), value2y(data->get(i    )) + yOffs, zOffs);
        Position p2(index2x(i + 1), value2y(data->get(i + 1)) + yOffs, zOffs + m_StripeWidth);
        geom->addRect(p1, p2, c);
    }
    geom->create_vao();
    checkError("Error create vao data");

    return geom;
}

Geometry *
Diagram::createBase(gfloat zOffs, gfloat yOffs)
{
    Geometry *geom = naviContext->createGeometry(GL_TRIANGLES);
    // Base
    Color c(0.2f, 0.2f, 0.2f);
    Position p1(index2x(0       ), value2y(0.f)-0.002f, zOffs);
    Position p2(index2x(m_size-1), value2y(0.f)-0.002f, zOffs + m_StripeWidth);
    geom->addRect(p1, p2, c);

    Position p3(index2x(0), value2y(0.f) - 0.002f, zOffs);
    Position p4(index2x(0), value2y(1.0f) - 0.002f, zOffs + m_StripeWidth);
    geom->addRect(p3, p4, c);
    geom->create_vao();
    checkError("Error create vao base");

    return geom;
}

void
Diagram::fill_buffers(guint idx, const std::shared_ptr<Buffer<double>> &data, Gdk::RGBA *color)
{
    if (idx < m_values.size()) {
        Geometry *old = m_values[idx]; // Save buffers so they can be freed, after new allocation
        if (old != nullptr) {
            //std::cerr << "Free oldPrim " << oldPrim->m_vao << " / " << oldPrim->m_index << std::endl;
            delete old;
        }
    }

    Geometry *geo = fill_data(data, color, 0.0f, -0.001f*idx);
    getBase()->addGeometry(geo);
    while (idx >= m_values.size()) {
        m_values.push_back(nullptr);
    }
    m_values[idx] = geo;
}

Geometry *
Diagram::getBase()
{
    if (base == nullptr) {
        base = createBase(0.0f, 0.0f);
    }
    return base;
}

void
Diagram::setMaxs(const Glib::ustring &pmax, const Glib::ustring &smax)
{
    if (m_pmax == nullptr) {
        m_pmax = m_textCtx->createText(naviContext, m_font);
        Position ptm(-1.0f, m_diagramHeight - m_font->getLineHeight() * 0.004f, 0.0f);
        m_pmax->setPosition(ptm);
        m_pmax->setScale(0.006f);
        getBase()->addGeometry(m_pmax);
    }
    if (m_smax == nullptr) {
        m_smax = m_textCtx->createText(naviContext, m_font);
        Position pti(-1.0f, m_diagramHeight - 2.0f*m_font->getLineHeight() * 0.004f, 0.0f);
        m_smax->setPosition(pti);
        m_smax->setScale(0.006f);
        getBase()->addGeometry(m_smax);
    }
    m_pmax->setText(pmax);
    m_smax->setText(smax);
    //Position &pos = getBase()->getPos();
    //Position ptn(pos.x-1.0f, pos.y + m_diagramHeight - m_font->getLineHeight() * 0.007f, pos.z);
    //m_font->display(m_sname, naviContext, persView, ptn, 0.010f);
    //checkError("display name");
//    Position ptm(pos.x-2.0f, pos.y + m_diagramHeight - m_font->getLineHeight() * 0.004f, pos.z);
//    m_font->display(pmax, naviContext, persView, ptm, 0.006f);
//    checkError("display pmax");
//    Position pti(pos.x-2.0f, pos.y + m_diagramHeight - 2.0f*m_font->getLineHeight() * 0.004f, pos.z);
//    m_font->display(smax, naviContext, persView, pti, 0.006f);
//    checkError("display smax");
}
