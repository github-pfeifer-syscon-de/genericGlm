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


#include "TextTreeGeometry.hpp"
#include "TreeNode.hpp"
#include "TextContext.hpp"


TextTreeGeometry::TextTreeGeometry(TreeNode *treeNode, Font *font, GeometryContext *_ctx)
: TreeGeometry::TreeGeometry(GL_TRIANGLES, _ctx, treeNode)
, m_font{font}
, m_text{nullptr}
{
}

TextTreeGeometry::~TextTreeGeometry()
{
    removeText();
}

void
TextTreeGeometry::createText(NaviContext *_ctx, TextContext *txtCtx) {
    if (m_text == nullptr) {
        m_text = txtCtx->createText(_ctx, m_font);
        Glib::ustring dName = m_treeNode->getDisplayName();
        m_text->setText(dName);
        m_text->setScale(0.0040f);
        Position p2 = getTreePos();
        m_text->setPosition(p2);
        addGeometry(m_text);   // this will care for transfrom if piece is moved
        m_text->setVisible(m_treeNode->isSelected());
    }
    // direct display will not use the correct (concat) transform
    //m_text->display(persView);
}

void
TextTreeGeometry::removeText() {
    if (m_text != nullptr) {
        delete m_text;
        m_text = nullptr;
    }
}


void
TextTreeGeometry::setTextVisible(bool visible)
{
    if (m_text != nullptr) {
        m_text->setVisible(visible);
    }
}
