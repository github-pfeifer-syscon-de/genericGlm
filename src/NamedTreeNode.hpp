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

#include <glibmm.h>
#include <map>
#include <memory>
#include <NaviContext.hpp>
#include <TextContext.hpp>
#include <Geometry.hpp>
#include <Text.hpp>
#include <Font.hpp>

class NamedTreeNode;

typedef std::shared_ptr<NamedTreeNode> ptrNamedTreeNode;

class NamedTreeNode
{
public:
    NamedTreeNode(const Glib::ustring& name, const Glib::ustring& key);
    explicit NamedTreeNode(const NamedTreeNode& orig) = delete;
    virtual ~NamedTreeNode();

    Glib::ustring getDisplayName() const;
    Glib::ustring getKey() const;
    ptrNamedTreeNode getChild(const Glib::ustring& key);
    void add(const ptrNamedTreeNode& pNamedTreeNode);
    std::list<ptrNamedTreeNode> getChildren();
    Geometry* getGeo();
    void createLineDown(NaviContext *shaderContext, float y);
    Geometry* getTreeGeometry(NaviContext *shaderContext
            ,TextContext *txtCtx
            ,Font* pFont
            ,Geometry* pParentGeo);
    virtual bool isMarkGeometryUpdated() = 0;
    virtual void updateMarkGeometry(NaviContext *shaderContext) = 0;
    float render(NaviContext *shaderContext
                ,TextContext *txtCtx
                ,Font* pFont
                ,Geometry* pParentGeo);
    void setParent(NamedTreeNode* parent);
protected:
    Glib::ustring m_name;
    Glib::ustring m_key;
    std::map<Glib::ustring, ptrNamedTreeNode> m_children;
    Geometry* m_geo{nullptr};
    Geometry* m_lineDown{nullptr};
    NamedTreeNode* m_parent{nullptr};
    float m_lastY{0.0f};
    static constexpr auto LINE_COLOR = 0.8f;
    static constexpr auto NODE_INDENT = 0.2f;
    static constexpr auto NODE_LINESPACE = -0.2f;

private:

};

//#include "NetNode.hpp"

//class NetNode;

//template class NamedTreeNode<NetNode>;
