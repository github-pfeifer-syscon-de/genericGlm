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

#include <glib-2.0/glib.h>
#include <memory>


template<class T>
class Buffer
{
public:
    Buffer(guint _size);
    Buffer(const Buffer& orig);
    virtual ~Buffer();

    void roll();
    void set(T _value);
    void add(const std::shared_ptr<Buffer<T>> &values);
    void set(guint idx, T _value);
    T sum();
    void refreshSum();
    T get(guint idx) const;
    T getMax() const;
    void reset();
private:
    guint m_size;
    T *m_data;
    T m_sum;
};
