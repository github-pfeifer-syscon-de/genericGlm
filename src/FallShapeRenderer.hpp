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
#include "Geometry.hpp"
#include "NaviContext.hpp"
#include "TreeRenderer.hpp"

class TextContext;

class FallShapeRenderer : public TreeRenderer {
public:
    FallShapeRenderer();
    virtual ~FallShapeRenderer();

    TreeGeometry *create(TreeNode *_root, NaviContext *_ctx, TextContext *textCtx, Font *font) override;
    void createFallShape(TreeNode *node, Position& start, double size, NaviContext *_ctx, TextContext *textCtx, Font *font, int depth);
    double getSingleSize(double size, TreeNode *node, TreeNode *childNode);
    double getDistIncrement() override {
        return shapeStep;
    }
    double getShapeForwardGap() {
        return shapeGap;
    }
    double getShapeSideGap() {
        return shapeGap / 2.0;
    }

    bool isFallDepth(int depth) {
        return depth > m_depth_fall;
    }
protected:
    double calculateLeafCount(TreeNode *node, double dist, double distInc);
    double shapeStep;
    double shapeGap;
    int m_depth_fall;

private:

};

