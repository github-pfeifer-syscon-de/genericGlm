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

#include <gtkmm.h>

#include "Geometry.hpp"
#include "Matrix.hpp"
#include "NaviContext.hpp"
#include "MarkContext.hpp"

class Scene : public GeometryDestructionListener
{
public:
    Scene();
    virtual ~Scene();

    virtual Position getIntialPosition() = 0;
    virtual float getZNear();
    virtual float getZFar();
    virtual float getViewAngleDegree();
    virtual Rotational getInitalAngleDegree() = 0;
    virtual guint32 getAnimationMs();
    virtual void init(Gtk::GLArea *glArea) = 0;
    virtual void draw(Gtk::GLArea *glArea, Matrix &proj, Matrix &view) = 0;
    virtual void unrealize() = 0;
    void unrealizeMark();
    virtual Matrix getLookAt(Position &viewPos, Vector &direction, Vector &up);
    virtual gboolean init_shaders(Glib::Error &error) = 0;
    virtual bool on_motion_notify_event(GdkEventMotion* event, float mx, float my);
    virtual Geometry *on_click_select(GdkEventButton* event, float mx, float my);
    virtual bool on_click(GdkEventButton* event, float mx, float my);
    virtual Gdk::EventMask getAddEventMask();
    virtual bool needsAnimation();
    Geometry *getSelected();
    void setSelected(Geometry *);
    void showMark(Gtk::GLArea *glArea, Matrix &proj, Matrix &view);
    Geometry *createMark();
    virtual void on_resize(int width, int height);
    virtual unsigned int getSampling();
    void createMarkContext();
    virtual bool selectionChanged(Geometry *prev_selected, Geometry *selected);
    void geometryDestroyed(Geometry *node)  override;
    virtual bool scroll(GdkEventScroll* event);
protected:

    Geometry *createMark(MarkContext *markContext);
    MarkContext *m_markContext;
    Geometry *m_mark;
    Geometry *m_selected;
    Matrix m_projView;
private:

};

