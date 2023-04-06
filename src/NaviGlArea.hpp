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
#include "Scene.hpp"
#include "FbShaderContext.hpp"

class NaviGlArea : public Gtk::GLArea {
public:
    NaviGlArea(Scene* scene);
    virtual ~NaviGlArea();
    bool on_leave_notify_event(GdkEventCrossing* crossing_event) override;
    virtual bool on_motion_notify_event(GdkEventMotion* event) override;
    bool on_scroll_event(GdkEventScroll* event) override;
    bool on_button_press_event(GdkEventButton* event) override;
    bool on_button_release_event(GdkEventButton* event) override;
    virtual void on_realize() override;
    void updateView();
    virtual float getAspect();
    virtual void on_resize(int width, int height) override;
    bool on_render(const Glib::RefPtr<Gdk::GLContext> &context) override;
    void on_unrealize() override;

    void update_timer();
    gboolean view_update();
    Matrix &getProjection();
    Matrix &getView();
    Rotational &getRotation();
    void setRotation(Rotational &rot);
    void set_clear_color(const Color& clear_color);

protected:
    Scene* scene;       // to make override work we need a pointer...
    gint m_mouseX ,m_mouseY;
    Rotational m_rot;
    Vector m_viewPos;	/* view position */
    Matrix m_projection;       /* projection matrix */
    Matrix m_view;	/* view matrix used for navigation */
    gint64 m_sum;       // Sum (us) used to draw scene
    guint m_cnt;        // Cnt of scene redraws requested
    sigc::connection m_timer;  // Timer for animation
    GLuint m_glTimer;   // gl-timer to measure timing
    FbShaderContext *m_fbctx;

    gboolean init_shaders(Glib::Error &error);
    void changeSelection(GdkEventButton* event);
private:

};

