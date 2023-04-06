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

#pragma once


#include "Geometry.hpp"

class TextContext;

typedef class TreeNode TreeNode;

class TreeGeometry : public Geometry {
public:
    TreeGeometry(GLenum type, GeometryContext *ctx, TreeNode *treeNode);
    virtual ~TreeGeometry();

    virtual bool match(const TreeGeometry *treeGeom) = 0;
    virtual void createText(NaviContext *_ctx, TextContext *txtCtx) = 0;
    virtual void removeText() = 0;
    virtual void setTextVisible(bool visible) = 0;

    void setTreeNode(TreeNode *treeNode);
    TreeNode *getTreeNode();

protected:
    TreeNode *m_treeNode;
private:

};
