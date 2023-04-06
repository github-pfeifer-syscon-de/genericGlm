/*
 * Copyright (C) 2021 rpf
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

#include <string>
#include <vector>

class StringUtils {
public:
    virtual ~StringUtils();

    // trim from start (in place)
    static void ltrim(std::string &s);

    // trim from end (in place)
    static void rtrim(std::string &s);

    // trim from both ends (in place)
    static void trim(std::string &s);

    static std::string lower(const std::string &s, int start = 0);

    static std::wstring from_bytesUtf8(const char *in);
    static std::wstring from_bytesUtf8(const std::string &in);

    static const char *weekday(int day);
    static void split(const std::string &line, char delim, std::vector<std::string> &ret);

private:
    StringUtils();

};

#ifndef ARRAYSIZE
#define ARRAYSIZE(arr) (sizeof(arr)/sizeof(arr[0]))
#endif

