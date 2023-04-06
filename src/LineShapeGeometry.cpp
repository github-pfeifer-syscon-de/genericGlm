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

#include "LineShapeGeometry.hpp"

#include <math.h>
#include <cstdlib>

LineShapeGeometry::LineShapeGeometry(Position& start, double width, double size, TreeNode *treeNode, int depth, Font *font, GeometryContext *_ctx)
: TextTreeGeometry::TextTreeGeometry(treeNode, font, _ctx)
, m_start{start}
, m_width{width}              // allocated x
, m_size{size}                // used size
, m_stage{treeNode->getStage()}
, m_load{treeNode->getLoad()}
, m_depth{depth}
, m_line{nullptr}
{
    setRemoveChildren(false);   // as we keep track of children in our structure (and need them to keep transform) do not delete them
}

LineShapeGeometry::~LineShapeGeometry()
{
    if (m_line != nullptr) {
        delete m_line;
        m_line = nullptr;
    }
}

void
LineShapeGeometry::create(TreeNode *node, LineShapeRenderer &_renderer) {
    double fwdSize = _renderer.getDistIncrement() - _renderer.getShapeForwardGap();
    Vector forward = Vector(0.0f, 0.0f, fwdSize);
    double xSize = m_size;
    if (xSize > _renderer.getShapeSideGap()) {
        xSize -= _renderer.getShapeSideGap();
    }

    m_mid = m_start;
    m_mid.x += m_width / 2.0 ;
    Position end(m_mid);
    end.x += xSize;
    m_treePos = m_mid;               // position used to render text
    m_treePos.z -= _renderer.getDistIncrement() / 2.0;
    Vector depthOffs = Vector(0.0f, -0.1f, 0.0f);
    Vector nt(0.0f, -1.0f, 0.0f);   // normal top
    Vector nf(0.0f,  0.0f, 1.0f);    // normal front
    Vector nb(0.0f,  0.0f, -1.0f);    // normal back
    Vector nr(-1.0f, -0.0f, 0.0f);    // normal right
    Vector nl( 1.0f, -0.0f, 0.0f);    // normal left
    Position p0(m_mid);
    Position p1(m_mid + forward);
    m_mid.x += xSize / 2.0;     // for later connecting adjust to middle

    Position p2(end);
    Position p3(end + forward);
    Position p4(p0 + depthOffs);
    Position p5(p1 + depthOffs);
    Position p6(p2 + depthOffs);
    Position p7(p3 + depthOffs);

    Color c0 = _renderer.pos2Color(m_mid.x, node->getStage(), m_load);
    addPoint(&p0, &c0, &nt);    // 0 top
    addPoint(&p1, &c0, &nt);    // 1
    addPoint(&p2, &c0, &nt);    // 2
    addPoint(&p3, &c0, &nt);    // 3
    addPoint(&p1, &c0, &nf);    // 4 front
    addPoint(&p3, &c0, &nf);    // 5
    addPoint(&p5, &c0, &nf);    // 6
    addPoint(&p7, &c0, &nf);    // 7
    addPoint(&p0, &c0, &nb);    // 8 back
    addPoint(&p4, &c0, &nb);    // 9
    addPoint(&p2, &c0, &nb);    // 10
    addPoint(&p6, &c0, &nb);    // 11
    addPoint(&p0, &c0, &nr);    // 12 left
    addPoint(&p1, &c0, &nr);    // 13
    addPoint(&p4, &c0, &nr);    // 14
    addPoint(&p5, &c0, &nr);    // 15
    addPoint(&p2, &c0, &nl);    // 16 right
    addPoint(&p3, &c0, &nl);    // 17
    addPoint(&p6, &c0, &nl);    // 18
    addPoint(&p7, &c0, &nl);    // 19

    addIndex(1, 3, 0);   // top near triangle
    addIndex(3, 2, 0);   // top far triangle
    addIndex(6, 7, 5);   // front low triangle
    addIndex(6, 5, 4);   // front upper triangle
    addIndex(9, 11, 10); // back low triangle
    addIndex(9, 10, 8);  // back upper triangle
    addIndex(15, 13, 12); // right low triangle
    addIndex(15, 12, 14); // right upper triangle
    addIndex(17, 19, 18); // left low triangle
    addIndex(17, 18, 16); // left upper triangle

    create_vao();

    TreeNode* parent = node->getParent();
    if (parent != nullptr) {
        if (auto parentLineShape = dynamic_cast<const LineShapeGeometry*>(parent->getTreeGeometry())) {
            Position pl0(parentLineShape->m_mid);
            pl0 += forward;
            Position pl1(pl0);
            pl1.z += _renderer.getShapeForwardGap() / 2.0;
            Position pl3(m_mid);
            Position pl2(pl3);
            pl2.z -= _renderer.getShapeForwardGap() / 2.0;
            m_line = new Geometry(GL_LINES, m_ctx);
            m_line->setMarkable(false);
            m_line->addLine(pl0, pl1, c0, &nt);
            m_line->addLine(pl1, pl2, c0, &nt);
            m_line->addLine(pl2, pl3, c0, &nt);
            m_line->create_vao();
            addGeometry(m_line);    // display together
        }
    }

}


bool
LineShapeGeometry::match(const TreeGeometry *treeGeo) {
    bool ret = FALSE;
    if(auto lineShape = dynamic_cast<const LineShapeGeometry*>(treeGeo)) {

        ret = lineShape->m_start == m_start &&
                lineShape->m_width == m_width &&
                lineShape->m_size == m_size &&
                lineShape->m_stage == m_stage &&
                lineShape->m_load == m_load;
//        if (!ret) {
//            std::cout << "inner " << sundisc->m_inner << "," << m_inner
//                      << " start " << sundisc->m_start << "," << m_start
//                      << " size " << sundisc->m_size << "," << m_size
//                      << " stage " << sundisc->m_stage << "," << m_stage
//                      << " load " << sundisc->m_load << "," << m_load
//                      << " ret " << ret << std::endl;
//        }
    }
    return ret;
}
