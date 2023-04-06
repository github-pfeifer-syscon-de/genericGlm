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


#include "SunDiscRenderer.hpp"
#include "SunDiscGeometry.hpp"
#include "TextContext.hpp"

SunDiscRenderer::SunDiscRenderer()
: radiusStep{0.4}	// disc thickness
, radiusGap{0.15}   // gap between discs outward , disks will be moved radiusGap/2 from origin
{
}

SunDiscRenderer::~SunDiscRenderer() {
}

TreeGeometry *
SunDiscRenderer::create(TreeNode *_root, NaviContext *_ctx, TextContext *textCtx, Font *font) {
    calculateLeafCount(_root, 1.0, radiusStep);

    Position pos(0.0f);
    setFullWidth(M_PI * 2.0);
    //std::cout << "-------" << std::endl;
    createSunDisc(0.0, _root, 0.0, getFullWidth(), _ctx, textCtx, pos, font, 0);

    return _root->getTreeGeometry();
}

/**
 * @Param inner inner radius
 * @Param node actual tree node
 * @Param start rad start of section
 * @Param size rad size of section
 * @Param idx coord index of geometry
 * @Param geo geometry to create
 */
void
SunDiscRenderer::createSunDisc(double inner, TreeNode *node, double start, double size, NaviContext *_ctx, TextContext *textCtx, Position &pos, Font *font, int depth) {

    TreeGeometry *tGeo = node->getTreeGeometry();
    SunDiscGeometry tempGeo(inner, start, size, node, pos, font, _ctx);
    bool toAdd = true;
    TreeNode *parentNode = node->getParent();
    if (tGeo == nullptr ||
        !tGeo->match(&tempGeo)) {
        SunDiscGeometry *sGeo = new SunDiscGeometry(inner, start, size, node, pos, font, _ctx);
        sGeo->create(node, *this);
        sGeo->createText(_ctx, textCtx);         // this will show text right from start if geometry changes
        node->setTreeGeometry(sGeo);
        if (parentNode == nullptr) {
            sGeo->setPosition(pos);
            _ctx->addGeometry(sGeo);        // required for clicking
            toAdd = false;
        }
        tGeo = sGeo;
    }
    if (toAdd
     && parentNode != nullptr) {        // in any case rebuild structure as parent may have changed
        Geometry *parentGeo = parentNode->getTreeGeometry();
        //std::cout << "add " << std::hex << sGeo << " to " << std::hex << parentGeo << std::endl;
        parentGeo->addGeometry(tGeo);       // replicate structure on geometries to allow breaking out tree parts
    }



    std::list<TreeNode *> &children = node->getChildren();
    if (children.size() > 0) {
        double st = start;
        for (auto p = children.begin(); p != children.end(); ++p) {
            TreeNode *childNode = (*p);
            double single = getSingleSize(size, node, childNode);
            createSunDisc(inner + radiusStep, childNode, st, single, _ctx, textCtx, pos, font, depth + 1);
            st += single;
        }
    }
}

// just distribute space between children evenly (unevenly sized leaves)
//double
//SunDiscRenderer::getSingleSize(double size, TreeNode *node, TreeNode *childNode) {
//    return size / (double)(node->getChildren().size());
//}

// distribute space between children depending on leaves
//   (evenly sized leaves (see also calculateLeafCount to size leaves similar))
double
SunDiscRenderer::getSingleSize(double size, TreeNode *node, TreeNode *childNode) {
    double ratio = childNode->getLeafCount() / node->getLeafCount();
    return size * ratio;
}

double
SunDiscRenderer::calculateLeafCount(TreeNode *node, double dist, double distInc) {
    double leafCount = 0.0;
    std::list<TreeNode *> &children = node->getChildren();
    if (children.size() > 0) {
        for (auto chld = children.begin(); chld != children.end(); ++chld) {
            TreeNode *chldNode = (*chld);
            leafCount += calculateLeafCount(chldNode, dist + distInc, distInc);
        }
    }
    else {
        leafCount += 1.0 / dist;  // scale by distance (distribute size better than by angle) smaller angel outwards but similar arc length ...
    }
    node->setLeafCount(leafCount);
    return leafCount;
}