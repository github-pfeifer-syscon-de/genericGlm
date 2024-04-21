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

#include "Geom2.hpp"
#include "Matrix.hpp"
#include "NaviContext.hpp"
#include "MarkContext.hpp"

class Scene 
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
    virtual psc::gl::aptrGeom2 on_click_select(GdkEventButton* event, float mx, float my);
    virtual bool on_click(GdkEventButton* event, float mx, float my);
    virtual Gdk::EventMask getAddEventMask();
    virtual bool needsAnimation();
    psc::gl::aptrGeom2 getSelected();
    void setSelected(const psc::gl::aptrGeom2&);
    void showMark(Gtk::GLArea *glArea, Matrix &proj, Matrix &view);
    psc::gl::aptrGeom2 createMark();
    virtual void on_resize(int width, int height);
    virtual unsigned int getSampling();
    void createMarkContext();
    virtual bool selectionChanged(const psc::gl::aptrGeom2& prev_selected, const psc::gl::aptrGeom2& selected);
    virtual bool scroll(GdkEventScroll* event);
protected:

    Geometry *createMark(MarkContext *markContext);
    MarkContext *m_markContext;
    psc::gl::aptrGeom2 m_mark;
    psc::gl::aptrGeom2 m_selected;
    Matrix m_projView;
private:

};

