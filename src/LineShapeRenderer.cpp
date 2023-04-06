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

#include <math.h>
#include <iostream>


#include "LineShapeRenderer.hpp"
#include "LineShapeGeometry.hpp"
#include "TextContext.hpp"

LineShapeRenderer::LineShapeRenderer()
: shapeStep{0.4}    // shape depth
, shapeGap{0.25}    // gap between shapes forward , side gap will be half of this
{
}

LineShapeRenderer::~LineShapeRenderer() {
}

TreeGeometry *
LineShapeRenderer::create(TreeNode *_root, NaviContext *_ctx, TextContext *textCtx, Font *font) {
    calculateLeafCount(_root, 1.0, shapeStep);

    Position start(0.0f);
    double width = 10.0;
    setFullWidth(width);
    double size = width / _root->getLeafCount();
    //std::cout << "-------" << std::endl;
    createLineShape(_root, start, width, size, _ctx, textCtx, font, 0);

    return _root->getTreeGeometry();
}

/**
 * @Param node actual tree node
 * @Param start start of process
 * @Param width allocated (according to children)
 * @Param size used (fixed for tree)
 * @Param idx coord index of geometry
 * @Param geo geometry to create
 */
void
LineShapeRenderer::createLineShape(TreeNode *node, Position& start, double width, double size, NaviContext *_ctx, TextContext *textCtx, Font *font, int depth) {

    TreeGeometry *tGeo = node->getTreeGeometry();
    LineShapeGeometry tempGeo(start, width, size, node, depth, font, _ctx);
    bool toAdd = true;
    TreeNode *parentNode = node->getParent();
    if (tGeo == nullptr ||
        !tGeo->match(&tempGeo)) {
        LineShapeGeometry *sGeo = new LineShapeGeometry(start, width, size, node, depth, font, _ctx);
        sGeo->create(node, *this);
        sGeo->createText(_ctx, textCtx);         // this will show text right from start if geometry changes
        node->setTreeGeometry(sGeo);
        if (parentNode == nullptr) {
            sGeo->setPosition(start);
            _ctx->addGeometry(sGeo);        // required for clicking
            toAdd = false;
        }
        tGeo = sGeo;
    }
    if (toAdd &&
        parentNode != nullptr) {        // in any case rebuild structure as parent may have changed
        Geometry *parentGeo = parentNode->getTreeGeometry();
        //std::cout << "add " << std::hex << tGeo << " to " << std::hex << parentGeo << std::endl;
        parentGeo->addGeometry(tGeo);       // replicate structure on geometries to allow breaking out tree parts
    }

    std::list<TreeNode *> &children = node->getChildren();
    Vector forward = Vector(0.0f, 0.0f, shapeStep);
    Position childPos(start + forward);

    for (auto p = children.begin(); p != children.end(); ++p) {
        TreeNode *childNode = (*p);
        double cwidth = getSingleSize(width, node, childNode);
        createLineShape(childNode, childPos, cwidth, size, _ctx, textCtx, font, depth + 1);
        childPos.x += cwidth;
    }

}

// distribute space between children by number end nodes
double
LineShapeRenderer::getSingleSize(double size, TreeNode *node, TreeNode *childNode) {
    double ratio = childNode->getLeafCount() / node->getLeafCount();
    return size * ratio;
}

double
LineShapeRenderer::calculateLeafCount(TreeNode *node, double dist, double distInc) {
    double leafCount = 0.0;
    std::list<TreeNode *> &children = node->getChildren();
    if (children.size() > 0) {
        for (auto chld = children.begin(); chld != children.end(); ++chld) {
            TreeNode *chldNode = (*chld);
            leafCount += calculateLeafCount(chldNode, dist + distInc, distInc);
        }
    }
    else {
        leafCount += 1.0;  // end node just use count
    }
    node->setLeafCount(leafCount);
    return leafCount;
}