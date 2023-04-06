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



#include "Buffer.hpp"

template<class T>
Buffer<T>::Buffer(guint _size)
: m_size{_size}
, m_data{nullptr}
, m_sum{0}
{
    m_data = new T[m_size];
	reset();
}

template<class T>
Buffer<T>::Buffer(const Buffer& orig)
: m_size(orig.m_size)
, m_data(nullptr)
, m_sum(orig.m_sum)
{
    m_data = new T[m_size];
    for (guint i = 0; i < m_size; ++i) {
        m_data[i] = orig.m_data[i];
    }
}

template<class T>
Buffer<T>::~Buffer()
{
    if (m_data != nullptr) {
        delete[] m_data;
        m_data = nullptr;
    }
}

template<class T> void
Buffer<T>::set(T _value) {
    set(m_size-1, _value);
	m_sum += _value;	// account for added value
}

template<class T> void
Buffer<T>::add(const std::shared_ptr<Buffer<T>> &_values) {
    m_sum = 0;
    for (guint i = 0; i < m_size; ++i) {
        m_data[i] += _values->get(i);
		m_sum += m_data[i];
    }
}

template<class T> void
Buffer<T>::set(guint idx, T _value) {
    if (m_data != nullptr && idx < m_size) {
        m_data[idx] = _value;
    }
}

template<class T> T
Buffer<T>::get(guint idx) const {
    if (idx < m_size && m_data != nullptr) {
        return m_data[idx];
    }
    return 0;
}

template<class T> T
Buffer<T>::getMax() const {
    T max = 1;
    for (guint i = 0; i < m_size; i++) {
        if (max < get(i))
            max = get(i);
    }
    return max;
}

template<class T> void
Buffer<T>::roll()
{
    if (m_data != nullptr) {
        m_sum -= m_data[0];
        for (guint i = 0; i < m_size - 1; ++i)
            m_data[i] = m_data[i + 1];
         m_data[m_size - 1] = 0;        // if we wont get updates use 0
    }
}

template<class T> T
Buffer<T>::sum()
{
    //T tsum = 0;
    //for (int i = 0; i < m_size; ++i)
    //    tsum += m_data[i];
    //if (abs(m_sum - tsum) > 0.00001) {
    //    std::cout << "Missmatching sum rec " << m_sum << " calc " << tsum << std::endl;
    //}
    return m_sum;
}

template<class T> void
Buffer<T>::refreshSum()
{
    m_sum = 0;
    if (m_data != nullptr) {
        for (guint i = 0; i < m_size; ++i)
            m_sum += m_data[i];
    }
}

template<class T> void
Buffer<T>::reset()
{
    for (guint i = 0; i < m_size; ++i) {
        m_data[i] = 0;
    }
	m_sum = 0;
}

template class Buffer<double>;
template class Buffer<guint64>;