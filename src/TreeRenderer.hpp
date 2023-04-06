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

#include "TreeNode.hpp"
#include "TreeGeometry.hpp"

class TreeRenderer {
public:
    TreeRenderer();
    virtual ~TreeRenderer();

    virtual TreeGeometry *create(TreeNode *_root, NaviContext *_ctx, TextContext *textCtx, Font *font) = 0;
    virtual double getDistIncrement() = 0;

    void setFullWidth(double _fullWidth);
    double getFullWidth();

    Color pos2Color(double pos, int stage, float load);

private:
    double m_fullWidth;
};

