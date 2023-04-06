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

#include "TextTreeGeometry.hpp"
#include "SunDiscRenderer.hpp"

class TextContext;

class SunDiscGeometry : public TextTreeGeometry {
public:
    SunDiscGeometry(double inner, double start, double size, TreeNode *treeNode, Position &pos, Font *font, GeometryContext *_ctx);
    virtual ~SunDiscGeometry();

    void create(TreeNode *node, SunDiscRenderer &_renderer);
    bool match(const TreeGeometry *treeGeom) override;
protected:
    double m_start;
    double m_size;
    double m_inner;
    int m_stage;
    float m_load;
    Position m_pos;
private:

};
