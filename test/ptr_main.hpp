/* -*- Mode: c++; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * Copyright (C) 2024 RPf <gpl3@pfeifer-syscon.de>
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


class Base
{
public:
    Base(int n);
    explicit Base(const Base& orig) = delete;
    virtual ~Base();

    virtual void dummy();
    void base();
    int get();
protected :
    int m_val;
};

class Test
: public Base
{
public:
    Test(int n);
    explicit Test(const Test& orig) = delete;
    virtual ~Test();

    void dummy() override;
    void test();
};

class Unrelated
{
public:
    Unrelated() = default;
    explicit Unrelated(const Unrelated& orig) = delete;
    ~Unrelated() = default;
};