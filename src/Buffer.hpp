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

#include <cstdint>
#include <memory>
#include <vector>

template<class T>
class Buffer
{
public:
    Buffer(uint32_t _size);
    Buffer(const Buffer& orig);
    virtual ~Buffer() = default;

    inline uint32_t getSize() {
        return m_data.size();
    }
    void roll();
    void set(T _value);
    void add(const std::shared_ptr<Buffer<T>> &values);
    void set(uint32_t idx, T _value);
    T sum();
    void refreshSum();
    T get(uint32_t idx) const;
    T getMax() const;
    void reset();
private:
    std::vector<T> m_data;
    T m_sum;
};
