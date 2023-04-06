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


#include "TreeRenderer.hpp"

TreeRenderer::TreeRenderer()
{
}


TreeRenderer::~TreeRenderer()
{
}

void
TreeRenderer::setFullWidth(double _fullWidth)
{
    m_fullWidth = _fullWidth;
}

double
TreeRenderer::getFullWidth()
{
    return m_fullWidth;
}

Color
TreeRenderer::pos2Color(double pos, int stage, float load)
{
    float coffs = 0.2f + (float)pos / (m_fullWidth * 1.2);   // make adjacent piece distinguishable by color (mid is 0 .. 2pi)
    Color c(coffs, 1.f, 1.0f - coffs);      // started is greenish
    switch (stage) {
        case TreeNode::STAGE_RUN:
            c.r = 0.2f + load;        // if running show load as color, green ... blue
            c.g = 1.0f - coffs;
            c.b = coffs;
            break;
        case TreeNode::STAGE_FIN:
        case TreeNode::STAGE_CLS:
            c.r = 1.0f;
            c.g = 0.0f;
            c.b = 0.0f;
            break;
    }

    return c;
}

