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
#include "FallShapeRenderer.hpp"

class TextContext;

class FallShapeGeometry : public TextTreeGeometry {
public:
    FallShapeGeometry(Position& start, double size, TreeNode *treeNode, int depth, Font *font, GeometryContext *_ctx);
    virtual ~FallShapeGeometry();

    void create(TreeNode *node, FallShapeRenderer &_renderer);
    bool match(const TreeGeometry *treeGeom) override;


protected:
    Position m_start;
    double m_size;
    int m_stage;
    float m_load;
    int m_depth;
private:

};
