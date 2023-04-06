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

#include "Scene.hpp"

Scene::Scene()
: m_markContext{nullptr}
, m_mark{nullptr}
, m_selected{nullptr}
{
}

Scene::~Scene()
{
}

float
Scene::getZNear()
{
    return 0.1f;
}

float
Scene::getZFar()
{
    return 100.0f;
}

float
Scene::getViewAngleDegree()
{
    return 60.0f;
}

guint32
Scene::getAnimationMs()
{
    return 0u;
}

bool
Scene::needsAnimation()
{
    return true;    // allows defining if animation is needed -> ret true
}


Matrix
Scene::getLookAt(Position &viewPos, Vector &direction, Vector &up)
{
    return glm::lookAt(
        viewPos,           // Camera is here
        viewPos+direction, // better than tweaking the sign of direction is change inital rotation to 180.
        up);
}

bool
Scene::on_motion_notify_event(GdkEventMotion* event, float mx, float my)
{
    return true;
}

Geometry *
Scene::on_click_select(GdkEventButton* event, float mx, float my)
{
    return nullptr;
}

void
Scene::createMarkContext()
{
    if (m_markContext == nullptr) {
        Glib::Error error;
        m_markContext = new MarkContext();
        bool ret = m_markContext->createProgram(error);
        if (error.gobj() != nullptr) {
            std::cout << "Error creating mark shader " << error.code() << " " << error.what() << std::endl;
        }
        if (!ret) {
            std::cout << "Error creating mark shader " << std::endl;
        }
    }
}

bool
Scene::on_click(GdkEventButton* event, float mx, float my)
{
    Geometry *selected = on_click_select(event, mx, my);
    if (selected != m_selected) {
        selectionChanged(m_selected, selected);
        if (m_selected != nullptr) {
            m_selected->removeDestructionListener(this);
        }
        if (selected != nullptr) {       // keep track of removed elements as they might be animated so they will vanish
            selected->addDestructionListener(this);
        }
        m_selected = selected;
        createMarkContext();
        if (m_mark != nullptr) {
            delete m_mark;
            m_mark = nullptr;
        }
        m_mark = createMark();
        return true;
    }
    return false;
}

bool
Scene::selectionChanged(Geometry *prev_selected, Geometry *selected) {
    // allow customized actions on selection
    return false;
}

Gdk::EventMask
Scene::getAddEventMask()
{
    return (Gdk::EventMask)0;
}

Geometry *
Scene::getSelected()
{
    return m_selected;
}

void
Scene::setSelected(Geometry *selected)
{
    m_selected = selected;
}

void
Scene::unrealizeMark()
{
    //std::cout << "Scene::unrealize" << std::endl;
    if (m_mark != nullptr) {
        delete m_mark;
        m_mark = nullptr;
    }
    if (m_markContext != nullptr) {
        delete m_markContext;
    }
}

void
Scene::geometryDestroyed(Geometry *node) {
    if (node == m_selected) {
        m_selected = nullptr;   // avoid stall references
        if (m_mark != nullptr) {   // and remove related
            delete m_mark;
            m_mark = nullptr;
        }
    }
}

void
Scene::showMark(Gtk::GLArea *glArea, Matrix &proj, Matrix &view)
{
    if (m_markContext != nullptr
     && m_mark != nullptr) {
        m_markContext->use();
        checkError("useMarkCtx");
        Matrix mv = proj * view;
        Matrix mvp = mv  * m_selected->getConcatTransform();
        m_markContext->setMvp(mvp);
        glLineWidth(getSampling()); // increase line with with sampling, so it shoud be visible
        m_mark->display(mv);
        checkError("markDisp");
        m_markContext->unuse();
        checkError("unuseMark");
    }
}

Geometry *
Scene::createMark()
{
    if (m_selected != nullptr
     && m_markContext != nullptr) {
        Position &min = m_selected->getModelMin();
        Position &max = m_selected->getModelMax();
        Geometry *mark = new Geometry(GL_LINES, m_markContext);
        // transfrom here has no meaning as we want a dynamic updated mark
        mark->setMarkable(false);
        Position pm0(min.x, min.y, min.z);
        Position pm1(max.x, min.y, min.z);
        Position pm2(max.x, max.y, min.z);
        Position pm3(min.x, max.y, min.z);
        Position pm4(min.x, min.y, max.z);
        Position pm5(max.x, min.y, max.z);
        Position pm6(max.x, max.y, max.z);
        Position pm7(min.x, max.y, max.z);
        Color c(1.0f);
        mark->addPoint(&pm0, &c, nullptr, nullptr);
        mark->addPoint(&pm1, &c, nullptr, nullptr);
        mark->addPoint(&pm2, &c, nullptr, nullptr);
        mark->addPoint(&pm3, &c, nullptr, nullptr);
        mark->addPoint(&pm4, &c, nullptr, nullptr);
        mark->addPoint(&pm5, &c, nullptr, nullptr);
        mark->addPoint(&pm6, &c, nullptr, nullptr);
        mark->addPoint(&pm7, &c, nullptr, nullptr);
        mark->addIndex(0, 1);   // front
        mark->addIndex(1, 2);
        mark->addIndex(2, 3);
        mark->addIndex(3, 0);
        mark->addIndex(0, 4);   // bottom
        mark->addIndex(4, 5);
        mark->addIndex(5, 1);
        mark->addIndex(5, 6);   // right
        mark->addIndex(6, 2);
        mark->addIndex(6, 7);   // top
        mark->addIndex(7, 3);
        mark->addIndex(4, 7);   // left
        mark->create_vao();
        return mark;
    }
    return nullptr;
}

void
Scene::on_resize(int width, int height)
{
}

unsigned int
Scene::getSampling()
{
    return 2;   // applies in x and y so with 2 we get 4 samples, and this is the useful max with the texture initialize with GL_LINEAR
}

bool
Scene::scroll(GdkEventScroll* event)
{
    return FALSE;
}