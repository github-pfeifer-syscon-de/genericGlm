/* -*- Mode: c++; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4; coding: utf-8; -*-  */
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

#include <iostream>
#include <StringUtils.hpp>

#include "NamedTreeNode.hpp"


NamedTreeNode::NamedTreeNode(const Glib::ustring& name, const Glib::ustring& key)
: m_name{name}
, m_key{key}
{
}


NamedTreeNode::~NamedTreeNode()
{
    //std::cout << std::scouce_location::current() << m_name << std::endl;
    // order matters cleanup bottom to top!
    m_children.clear();
    if (m_geo) {
        delete m_geo;   // will remove from depended (parent) as well
        m_geo = nullptr;
    }
}

Glib::ustring
NamedTreeNode::getDisplayName() const
{
    return m_name;
}

Glib::ustring
NamedTreeNode::getKey() const
{
    return m_key;
}

ptrNamedTreeNode
NamedTreeNode::getChild(const Glib::ustring& key)
{
    ptrNamedTreeNode node;
    auto entry = m_children.find(key);
    if (entry != m_children.end()) {
        node = (*entry).second;
    }
    return node;
}

void
NamedTreeNode::add(const ptrNamedTreeNode& node)
{
    node->setParent(this);
    m_children.insert(std::pair(node->getKey(), node));
}

void
NamedTreeNode::setParent(NamedTreeNode* parent)
{
    m_parent = parent;
}

std::list<ptrNamedTreeNode>
NamedTreeNode::getChildren()
{
    std::list<ptrNamedTreeNode> ret;
    for (auto netchld : m_children) {
        ret.push_back(netchld.second);
    }
    return ret;
}

Geometry*
NamedTreeNode::getGeo()
{
    return m_geo;
}


void
NamedTreeNode::createLineDown(NaviContext *shaderContext, float y)
{
    if (m_children.size() == 0) {
        if (m_lineDown) {
            delete m_lineDown;
            m_lastY = 0.0f;
        }
    }
    else {
        if (!m_lineDown
          ||m_lastY != y)  {
            if (m_lineDown) {
                delete m_lineDown;
            }
            m_lineDown = new Geometry(GL_LINES, shaderContext);
            Position p1{0.0f, 0.0f, 0.0f};
            Position p2{0.0f, y, 0.0f};
            Color c(LINE_COLOR, LINE_COLOR, LINE_COLOR);
            m_lineDown->addLine(p1, p2, c);
            m_lineDown->create_vao();
            m_geo->addGeometry(m_lineDown);
            m_lastY = y;
        }
    }
}

Geometry*
NamedTreeNode::getTreeGeometry(
                 NaviContext *shaderContext
                ,TextContext *txtCtx
                ,Font* pFont
                ,Geometry* pParentGeo)
{
    bool created = false;
    if (!m_geo) {
        m_geo = new Geometry(GL_TRIANGLES, shaderContext);
        auto text = txtCtx->createText(shaderContext, pFont);
        text->setText(getDisplayName());
        text->setScale(0.005f);
        Position pt(0.10f, -0.05f, 0.0f);
        text->setPosition(pt);
        m_geo->addGeometry(text);
        if (m_parent) {
            auto lineLeft = new Geometry(GL_LINES, shaderContext);
            Position p1{0.0f, 0.02f, 0.0f};
            Position p2{-NamedTreeNode::NODE_INDENT, 0.02f, 0.0f};
            Color c(LINE_COLOR, LINE_COLOR, LINE_COLOR);
            lineLeft->addLine(p1, p2, c);
            lineLeft->create_vao();
            m_geo->addGeometry(lineLeft);
        }
        if (m_lineDown) {
            m_geo->addGeometry(m_lineDown);
        }
        if (pParentGeo) {
            pParentGeo->addGeometry(m_geo);
        }
        created = true;
    }
    if (created
     || isMarkGeometryUpdated()) {
        updateMarkGeometry(shaderContext);
    }

    return m_geo;
}

float
NamedTreeNode::render(NaviContext *shaderContext
                ,TextContext *txtCtx
                ,Font* pFont
                ,Geometry* pParentGeo)
{
    auto treeGeo = getTreeGeometry(shaderContext, txtCtx, pFont, pParentGeo);
    Position pos{NODE_INDENT, 0.0, 0.0};
    float lastY = 0.0f;
    for (auto chld : getChildren()) {
        pos.y += NODE_LINESPACE;
        auto chldGeo = chld->getTreeGeometry(shaderContext, txtCtx, pFont, treeGeo);
        chldGeo->setPosition(pos);
        lastY = pos.y;
        pos.y += chld->render(shaderContext, txtCtx, pFont, treeGeo);
    }
    createLineDown(shaderContext, lastY);
    return pos.y;   // as we start from 0 return the used space aka. length
}



