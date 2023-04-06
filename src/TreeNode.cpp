/*
 * Copyright (C) 2019 rpf
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

#include "TreeNode.hpp"

TreeNode::TreeNode()
: m_parent{nullptr}
, children()
, m_treeGeometry{nullptr}
, m_selected{false}
{
}

TreeNode::~TreeNode()
{
    if (m_parent != nullptr) {       // on deletion remove from structure, so we avoid dangling pointers...
        m_parent->remove(this);
    }
    for (auto chld = children.begin(); chld != children.end(); ++chld) {
        (*chld)->m_parent = nullptr;    // invalidate any reference to me
    }
    children.clear();
    removeGeometry();
}

void
TreeNode::add(TreeNode *_child)
{
    children.push_back(_child);
}

void
TreeNode::remove(TreeNode *_child)
{
    children.remove(_child);
    _child->m_parent = nullptr;
}

void
TreeNode::setParent(TreeNode *_parent)
{
    if (m_parent == _parent) {
        return;
    }
    if (m_parent != nullptr) {
        m_parent->remove(this);
    }
    m_parent = _parent;
    if (m_parent != nullptr) {
        m_parent->add(this);
        m_selected = m_parent->isSelected();
    }
}

void
TreeNode::setLeafCount(double leafCount) {
    m_leafCount = leafCount;
}

double
TreeNode::getLeafCount() {
    return m_leafCount;
}

void
TreeNode::setTextVisible(bool textVisisble) {
    if (m_treeGeometry != nullptr) {
        m_treeGeometry->setTextVisible(textVisisble);
    }
}

void
TreeNode::setTreeGeometry(TreeGeometry *_treeGeometry) {
    if (m_treeGeometry != nullptr) {
        if (_treeGeometry != nullptr) {
            Position &pos = m_treeGeometry->getPos();
            _treeGeometry->setPosition(m_treeGeometry->getPos());   // inherit a possibly modified transform
            _treeGeometry->setScale(m_treeGeometry->getScale());
            _treeGeometry->setRotation(m_treeGeometry->getRotation());
        }
        removeGeometry();
    }
    _treeGeometry->addDestructionListener(this);    // keep track if geometry will get destroyed by recursive destruction
    m_treeGeometry = _treeGeometry;
}

void
TreeNode::removeGeometry() {
    if (m_treeGeometry != nullptr) {
        m_treeGeometry->removeDestructionListener(this);
        delete m_treeGeometry;
        m_treeGeometry = nullptr;
    }
}

void
TreeNode::geometryDestroyed(Geometry *node) {
    if (m_treeGeometry == node) {
        m_treeGeometry = nullptr;  // invalidate reference
    }
}

bool
TreeNode::isSelected() {
    return m_selected;
}

void
TreeNode::setSelected(bool selected) {
    m_selected = selected;
    setTextVisible(selected);
    for (auto chld = children.begin(); chld != children.end(); ++chld) {
        TreeNode *p_chldNode = (*chld);
        p_chldNode->setSelected(selected);
    }
}

