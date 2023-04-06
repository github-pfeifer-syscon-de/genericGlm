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

#include "TreeGeometry.hpp"
#include "Text.hpp"


class TextTreeGeometry : public TreeGeometry
{
public:
    TextTreeGeometry(TreeNode *treeNode, Font *font, GeometryContext *_ctx);
    virtual ~TextTreeGeometry();

    Position &getTreePos() {
        return m_treePos;
    }
    Text *getText() {
        return m_text;
    }
    void setText(Text *_text) {
        m_text = _text;
    }
    void createText(NaviContext *_ctx, TextContext *txtCtx) override;
    void removeText() override;
    void setTextVisible(bool visible) override;

protected:
    Font *m_font;
    Text *m_text;
    Position m_treePos;     // text pos

private:

};
