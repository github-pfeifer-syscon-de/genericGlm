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

#include <list>

#include "TreeGeometry.hpp"

class TreeNode : public GeometryDestructionListener {
public:
    static const int STAGE_NEW = 0;
    static const int STAGE_RUN = 1;
    static const int STAGE_FIN = 2;
    static const int STAGE_CLS = 3;

    TreeNode();
    virtual ~TreeNode();

    void add(TreeNode *_child);
    void remove(TreeNode *_child);
    void setParent(TreeNode *_parent);
    virtual const char* getName() = 0;
    virtual int getStage() = 0;
    virtual float getLoad() = 0;
    virtual Glib::ustring getDisplayName() = 0;
    TreeNode *getParent() {
        return m_parent;
    }
    std::list<TreeNode *> &getChildren() {
        return children;
    }
    TreeGeometry *getTreeGeometry() {
        return m_treeGeometry;
    }
    void setTreeGeometry(TreeGeometry *_treeGeometry);
    void geometryDestroyed(Geometry *node) override;
    void setLeafCount(double leafCount);
    double getLeafCount();
    void removeGeometry();
    bool isSelected();
    void setSelected(bool selected);
protected:
    TreeNode *m_parent;
    std::list<TreeNode *> children;

    TreeGeometry *m_treeGeometry;
    double m_leafCount;
    bool m_selected;
private:
    void setTextVisible(bool textVisisble);
};

