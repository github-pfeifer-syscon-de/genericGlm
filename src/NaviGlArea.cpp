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

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
//#include <glm/gtc/quaternion.hpp>   // euler
//#include <glm/gtx/euler_angles.hpp> // eulerAngleYXZ
#include <glm/trigonometric.hpp>  //radians
#include <fstream>

#include "NaviGlArea.hpp"

NaviGlArea::NaviGlArea(Scene* _scene)
: scene{_scene}
, m_mouseX{0}, m_mouseY{0}
, m_rot()
, m_viewPos{_scene->getIntialPosition()}
, m_projection{1.0f}
, m_view{1.0f}
, m_sum{0}
, m_cnt{0}
, m_glTimer{0}
, m_fbctx{nullptr}
{
    m_rot.add(scene->getInitalAngleDegree());
    //set_required_version (2, 0);
    //set_use_es(TRUE);
    set_auto_render(TRUE);
    set_has_depth_buffer(TRUE);
    /* reset the mvp matrix */
    m_projection = glm::perspective(
        glm::radians(scene->getViewAngleDegree()),  // The vertical Field of View, in radians: the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
        getAspect(),                // Aspect Ratio.
        scene->getZNear(),                    // Near clipping plane. Keep as big as possible, or you'll get precision issues.
        scene->getZFar());                    // Far clipping plane. Keep as little as possible.


    updateView();

    // the on_realize on_unrealize methods will work if the widget is included by gtk::manage
    //signal_realize().connect(sigc::mem_fun(*this, &NaviGlArea::gl_init));
    //signal_unrealize().connect(sigc::mem_fun(*this, &NaviGlArea::gl_finish));

    add_events( Gdk::EventMask::BUTTON_PRESS_MASK
               | Gdk::EventMask::BUTTON_MOTION_MASK
               | Gdk::EventMask::SCROLL_MASK
               | Gdk::EventMask::KEY_PRESS_MASK
               | Gdk::EventMask::LEAVE_NOTIFY_MASK
               | scene->getAddEventMask());
}


NaviGlArea::~NaviGlArea()
{
    if (scene)
        delete scene;
}


void
NaviGlArea::updateView()
{
    float verticalAngle = m_rot.phiRadians();
    float horizontalAngle = m_rot.thetaRadians();
    float cosVert = std::cos(verticalAngle);
    float sinVert = std::sin(verticalAngle);
    float sinHorz = std::sin(horizontalAngle);
    float cosHorz = std::cos(horizontalAngle);
    glm::vec3 direction(
        cosVert * sinHorz,
        sinVert,
        cosVert * cosHorz);
    glm::vec3 right = glm::vec3(
        std::sin(horizontalAngle - (float)M_PI_2),
        0.0f,
        std::cos(horizontalAngle - (float)M_PI_2));
    glm::vec3 up = glm::cross(right, direction);
    m_view = scene->getLookAt(m_viewPos, direction, up);
}

void
NaviGlArea::update_timer()
{
    if (scene->getAnimationMs() > 0) {
        sigc::slot<bool> slot = sigc::mem_fun(*this, &NaviGlArea::view_update);
        m_timer = Glib::signal_timeout().connect(slot, scene->getAnimationMs());
    }
}

gboolean
NaviGlArea::view_update()
{
    if (scene->needsAnimation()) {
        queue_draw();
    }
    return TRUE;
}

bool
NaviGlArea::on_leave_notify_event(GdkEventCrossing* crossing_event)
{
    // on leave point somewhere outside scene
    float mx = -1000.0f;
    float my = -1000.0f;
    GdkEventMotion event;
    event.state = crossing_event->state;
    event.x = crossing_event->x;
    event.y = crossing_event->y;
    event.type = crossing_event->type;
    scene->on_motion_notify_event(&event, mx, my);
    return TRUE;
}

bool
NaviGlArea::on_motion_notify_event(GdkEventMotion* event)
{
    bool btn1 = (event->state & Gdk::ModifierType::BUTTON1_MASK) != 0;
    bool btn2 = (event->state & Gdk::ModifierType::BUTTON2_MASK) != 0;
    bool btn3 = (event->state & Gdk::ModifierType::BUTTON3_MASK) != 0;

    Geometry *selected = scene->getSelected();
    if (btn3) {
        if (selected != nullptr) {
            Rotational r = selected->getRotation();
            r.add((event->x - m_mouseX) / 3.0f, (event->y - m_mouseY) / 3.0f, 0.0f);    // work differently than below
            //std::cout << "e.x " << r.getPhi() << " e.y " << r.getTheta()  << " e.z " << r.getPsi() << std::endl;
            selected->setRotation(r);
        }
        else {
            m_rot.add((event->y - m_mouseY) / 3.0f, (event->x - m_mouseX) / 3.0f, 0.0f);
        }
    }
    else if (btn2) {
        float dx = -(event->x - m_mouseX) / 50.0f;
        float dy =  (event->y - m_mouseY) / 50.0f;
        if (selected != nullptr) {
            Position p = selected->getPos();
            p.x -= dx;
            p.y -= dy;
            selected->setPosition(p);
        }
        else {
            m_viewPos.x += dx;
            m_viewPos.y += dy;
        }
    }
    if (btn1 || btn2 || btn3) {
        updateView();

        m_mouseX = event->x; // As the previous move is accounted set new "origin"
        m_mouseY = event->y;
        queue_draw();
    }
    else {
        // scale mouse to gl-out-cube -1 ... 1
        float mx = 2.0f * (event->x / (float)get_width() - 0.5f);
        float my = 2.0f * (0.5f - event->y / (float)get_height());
        scene->on_motion_notify_event(event, mx, my);
    }
    return TRUE;
}

bool
NaviGlArea::on_scroll_event(GdkEventScroll* event)
{
    Geometry *selected = scene->getSelected();
    if (selected) {
        Position p = selected->getPos();
        p.z += event->direction == GDK_SCROLL_UP ? -0.3f : 0.3f;
        selected->setPosition(p);
    }
    else {
        if (!scene->scroll(event)) {
            m_viewPos.z += event->direction == GDK_SCROLL_UP ? -1.1f : 1.1f;
        }
    }
    updateView();
    queue_draw();
    return TRUE;
}

bool
NaviGlArea::on_button_press_event(GdkEventButton* event)
{
    bool btn1 = (event->button == 1);
    if (btn1) {
        changeSelection(event);
    }
    else {
        m_mouseX = event->x;
        m_mouseY = event->y;
    }
    //g_warning("button: %dx%d btn:0x%04x", event->x, event->y, event->button);
    return TRUE;
}

void
NaviGlArea::changeSelection(GdkEventButton* event)
{
    // scale mouse to gl-out-cube -1 ... 1
    float mx = 2.0f * (event->x / (float)get_width() - 0.5f);
    float my = 2.0f * (0.5f - event->y / (float)get_height());

    make_current();
    if (scene->on_click(event, mx, my)) {
        queue_draw();
    }
}

bool
NaviGlArea::on_button_release_event(GdkEventButton* event)
{
    return TRUE;
}

float
NaviGlArea::getAspect() {
    return (float)get_width() / (float)get_height();
}


void
NaviGlArea::on_resize(int width, int height)
{
    Gtk::GLArea::on_resize(width, height);
    if (m_fbctx)
        m_fbctx->setup(width, height, scene->getSampling());
    scene->on_resize(width, height);
    // Keep aspect of model
    m_projection = glm::perspective(
        glm::radians(scene->getViewAngleDegree()),   // The vertical Field of View, in radians: the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
        getAspect(),                                // Aspect Ratio.
        scene->getZNear(),                           // Near clipping plane. Keep as big as possible, or you'll get precision issues.
        scene->getZFar());                           // Far clipping plane. Keep as little as possible.
}


void
NaviGlArea::set_clear_color(const Color& clear_color)
{
    if (m_fbctx) {
        m_fbctx->set_clear_color(clear_color);
    }
}

void
NaviGlArea::on_realize()
{
    Gtk::Widget::on_realize();
    /* we need to ensure that the GdkGLContext is set before calling GL API */
    make_current();

    /* if the GtkGLArea is in an error state we don't do anything */
    if (has_error())
        return;


    /* initialize the shaders and retrieve the program data */
    Glib::Error error;
    if (!scene->init_shaders(error)) {
        /* set the GtkGLArea in error state, so we'll see the error message
         * rendered inside the viewport
         */
        set_error(error);
        return;
    }
    if (scene->getSampling() > 1) {
        m_fbctx = new FbShaderContext();
        m_fbctx->createProgram(error);

        m_fbctx->setup(get_width(), get_height(), scene->getSampling());
    }

    scene->init(this);

    glClearDepthf(1.0f);
    checkError("clear depth");
    glClearColor(0.1, 0.1, 0.15, 1.0);  // has no meaning rendering will be done by fb
    checkError("clear color");
    update_timer();
}

void
NaviGlArea::on_unrealize()
{
    if (m_timer.connected()) {
        m_timer.disconnect(); // No more updating
    }
    make_current();
    scene->unrealize();     // somehow the virtual and override do not work by reference but by pointer (strange c++ wonderland) why is it fine on all the other invocations ???
    scene->unrealizeMark();
    if (m_fbctx != nullptr)
        delete m_fbctx;
    m_fbctx = nullptr;
#ifdef DEBUG
    std::cout << "Rate " << ((double)m_cnt/(double)m_sum*1.0e9) << " f/s ("
              << m_cnt << "f "
              << m_sum << "ns)" << std::endl;
#endif
    // Chain default handler
    Gtk::GLArea::on_unrealize();
}

bool
NaviGlArea::on_render(const Glib::RefPtr<Gdk::GLContext> &context)
{
    make_current();
    /* draw our object */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    checkError("clear");

    if (m_fbctx != nullptr) {
        m_fbctx->prepare();        // move this out of repaint ... sometimes, but need to keep it in updates
        checkError("renderBuf prep");
    }
#ifdef DEBUG
    if (m_glTimer == 0) {
        glGenQueries(1, &m_glTimer);
    }
    else {
        // Wait for all results to become available
        //  to prevent drawing cpu time do this for last run
        GLint available = 0;
        do {
            glGetQueryObjectiv(m_glTimer, GL_QUERY_RESULT_AVAILABLE, &available);
        } while (!available);
        GLuint timeElapsed = 0;
        glGetQueryObjectuiv(m_glTimer, GL_QUERY_RESULT, &timeElapsed);
        m_sum += timeElapsed; // end_time - start_time;
        m_cnt++;
    }
    glBeginQuery(GL_TIME_ELAPSED, m_glTimer);
#endif
    scene->draw(this, m_projection, m_view);
    scene->showMark(this, m_projection, m_view); // do this after as we need calculated transform
    checkError("renderBuf draw");
#ifdef DEBUG
    glEndQuery(GL_TIME_ELAPSED);
#endif
    if (m_fbctx != nullptr) {
        m_fbctx->done();
        checkError("renderBuf done");
        glFlush();
        attach_buffers();       // this is the game changer to get output !!!

        glViewport(0, 0, get_width(), get_height());    // need to keep updating !!! as viewport changes ...
        checkError("view viewport");

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        checkError("clear");

        m_fbctx->draw();
        checkError("renderBuf draw");
    }
    glFlush();  /* flush the contents of the pipeline, shoud be mostly harmless */
    return FALSE;
}

Matrix &
NaviGlArea::getProjection()
{
    return m_projection;
}

Matrix &
NaviGlArea::getView()
{
    return m_view;
}

Rotational &
NaviGlArea::getRotation()
{
    return m_rot;
}

void
NaviGlArea::setRotation(Rotational &rot)
{
    m_rot = rot;
}
