/* -*- Mode: c++; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4; coding: utf-8; -*-  */
/*
 * Copyright (C) 2024 RPf 
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


#include "TreeNode2.hpp"
#include "TextContext.hpp"
#include "TreeGeometry2.hpp"

namespace psc {
namespace gl {




TreeGeometry2::TreeGeometry2(GLenum type, GeometryContext *ct, const std::shared_ptr<TreeNode2>& treeNode)
: Geom2::Geom2(type, ct)
, m_treeNode{treeNode}
{
}

std::weak_ptr<TreeNode2>
TreeGeometry2::getTreeNode()
{
    return m_treeNode;
}



} /* namespace gl */
} /* namespace psc */
