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

#include "SunDiscGeometry.hpp"
#include <math.h>

SunDiscGeometry::SunDiscGeometry(double inner, double start, double size, TreeNode *treeNode, Position &pos, Font *font, GeometryContext *_ctx)
: TextTreeGeometry::TextTreeGeometry(treeNode, font, _ctx)
, m_start{start}
, m_size{size}
, m_inner{inner}
, m_stage{treeNode->getStage()}
, m_load{treeNode->getLoad()}
, m_pos{pos}
{
    setRemoveChildren(false);   // as we keep track of children in our structure (and need them to keep transform) do not delete them
}

SunDiscGeometry::~SunDiscGeometry()
{
}

void
SunDiscGeometry::create(TreeNode *node, SunDiscRenderer &_renderer) {
    short idx = 0;
    double outer = m_inner + _renderer.getDistIncrement() - _renderer.getRadiusGap();
    double end = m_start + m_size;
    double step = M_PI * 2.0 / 60.0;   // approximate circle by 60 sections or 120 triangles
    bool firstEnd = TRUE;

    double midR = m_start + m_size / 2.0;
    double si = sin(midR);
    double co = cos(midR);
    double xoffs = _renderer.getRadiusGap() / 2.0 * si;
    double zoffs = _renderer.getRadiusGap() / 2.0 * co;    // move cake pieces outward slightly
    float y = (m_inner) / 4.0f;
    m_treePos.x = m_inner * si;               // position used to render text
    m_treePos.y = y + 0.1f;
    m_treePos.z = m_inner * co;
    float yh = 0.1f;
    Vector nt(0.0f, -1.0f, 0.0f);   // normal top
    for (double i = m_start; i <= end; ) {
        si = sin(i);
        co = cos(i);
        Vector nsi(si, -0.4f, co);       // normal side inner
        Vector nso(si, -0.4f, -co);      // normal side outer
        Position p0(xoffs + m_inner * si, y, zoffs + m_inner * co);
        Position p1(xoffs + outer * si, y, zoffs + outer * co);
        Position p2(xoffs + m_inner * si, y - yh, zoffs + m_inner * co);
        Position p3(xoffs + outer * si, y - yh, zoffs + outer * co);

        Color c = _renderer.pos2Color(midR, node->getStage(), m_load);
        addPoint(&p0, &c, &nt);
        addPoint(&p1, &c, &nt);
        addPoint(&p0, &c, &nsi);
        addPoint(&p1, &c, &nso);
        addPoint(&p2, &c, &nsi);
        addPoint(&p3, &c, &nso);
        if (idx > 0) {
            addIndex(idx - 0, idx - 5, idx - 6);   // top left triangle
            addIndex(idx - 0, idx + 1, idx - 5);   // top right triangle
            addIndex(idx + 2, idx - 2, idx + 4);   // inner bottom triangle
            addIndex(idx + 2, idx - 4, idx - 2);   // inner top triangle
            addIndex(idx + 3, idx + 5, idx - 1);   // outer bottom triangle
            addIndex(idx - 1, idx - 3, idx + 3);   // outer top triangle
            if (!firstEnd) {
                addIndex(idx + 4, idx + 3, idx + 2);   // end cap top
                addIndex(idx + 4, idx + 5, idx + 3);   // end cap bottom
            }
        }
        else {
            addIndex(idx + 4, idx + 2, idx + 3);   // start cap top
            addIndex(idx + 4, idx + 3, idx + 5);   // start cap bottom
        }

        idx += 6;
        double next = end - i;
        if (step < next) {
            i += step;
        }
        else {
            if (firstEnd) {
                i += next;     // match exact end (needed to draw small triangles !)
                firstEnd = FALSE;
            }
            else {
                break;          // for not looping forever
            }
        }
    }
    create_vao();
}

bool
SunDiscGeometry::match(const TreeGeometry *treeGeo) {
    bool ret = FALSE;
    if(auto sundisc = dynamic_cast<const SunDiscGeometry*>(treeGeo)) {

        ret = sundisc->m_inner == m_inner &&
                sundisc->m_start == m_start &&
                sundisc->m_size == m_size &&
                sundisc->m_stage == m_stage &&
                sundisc->m_load == m_load;
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
