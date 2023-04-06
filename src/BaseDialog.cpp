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


#include "BaseDialog.hpp"

BaseDialog::BaseDialog(const char *name)
: Gtk::Dialog(name, Gtk::DialogFlags::DIALOG_MODAL | Gtk::DialogFlags::DIALOG_DESTROY_WITH_PARENT)
{
}

BaseDialog::~BaseDialog()
{
}

void
BaseDialog::add_widget2box(Gtk::Box *dest, const char *lbl, Gtk::Widget *toAdd, gfloat y_scale)
{
    Gtk::Frame *_frame = new Gtk::Frame(lbl);
    _frame->set_shadow_type(Gtk::ShadowType::SHADOW_NONE);
    Gtk::Alignment *_align = new Gtk::Alignment(0, 0, 0, y_scale);
    //gtk_alignment_set_padding(GTK_ALIGNMENT(_align), 4, 4, 4, 4);
    _frame->add(*_align);
    _align->add(*toAdd);
    dest->pack_start(*_frame, TRUE, TRUE, 0);
}