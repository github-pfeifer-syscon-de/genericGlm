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

// simple diagram display with a given number of values and a variable number of lines

#pragma once

#include <string>
#include <vector>

#include "Font.hpp"
#include "Buffer.hpp"
#include "NaviContext.hpp"
#include "TextContext.hpp"
#include "Text.hpp"

class Diagram
{
public:
    Diagram(guint _size, NaviContext *naviContext, TextContext *_textCtx);
    virtual ~Diagram();

    void setPosition(Position &pos);
    void setFont(Font *font);
    void setName(const Glib::ustring &sname);

    Geometry *fill_data(const std::shared_ptr<Buffer<double>> &data, Gdk::RGBA *color,
	gfloat zOffs, gfloat yOffs);

    void fill_buffers(guint idx, const std::shared_ptr<Buffer<double>> &data, Gdk::RGBA *color);
    void setMaxs(const Glib::ustring &pmax, const Glib::ustring &smax);

    gfloat getDiagramHeight() const {       // y
        return m_diagramHeight;
    }
    void setDiagramHeight(gfloat _height) {
        m_diagramHeight =  _height;
    }
    gfloat getDiagramDepth() const {        // z
        return m_StripeWidth;
    }
    void setDiagramDepth(gfloat _depth) {
        m_StripeWidth = _depth;
    }
    gfloat getDiagramWidth() const {        // x
        return m_diagramWidth;
    }
    void setDiagramWidth(gfloat _width) {
        m_diagramWidth = _width;
    }
protected:
    gfloat m_StripeWidth;
    gfloat m_diagramHeight;
    gfloat m_diagramWidth;
    Geometry *base;
    std::vector<Geometry *> m_values;
    Font *m_font;
    Text *m_sname;
    Text *m_pmax;
    Text *m_smax;

    std::wstring m_max;
    NaviContext *naviContext;
    TextContext *m_textCtx;
    gfloat value2y(gfloat val);
    gfloat index2x(guint i);
private:
    Geometry *createBase(gfloat zOffs, gfloat yOffs);
    Geometry *getBase();
    guint m_size;       // number of data points must be shared with buffer
};

