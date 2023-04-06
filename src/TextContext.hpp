/*
 * Copyright (C) 2018 rpf
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

#include "NaviContext.hpp"

class Text;



class TextContext : public NaviContext
{
public:
    TextContext(GLenum type);
    virtual ~TextContext();

    void setColor(Color &c);
    bool useNormal() override;
    bool useColor() override;
    bool useUV() override;
    bool createProgram(Glib::Error &error);
    void setTexture(int texture);
    // create a text with matching parameters for this 
    //   "belonging" to ctx to allow rendering in a batch
    //   using this ctx for rendering
    Text *createText(NaviContext *ctx, Font *font);
protected:
    void updateLocation() override;

private:
    GLenum m_type;
    GLint m_color_location;
    GLint m_texture_location;
};
