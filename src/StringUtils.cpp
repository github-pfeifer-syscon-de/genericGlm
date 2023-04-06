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

#include <wchar.h>
#include <locale>
#include <codecvt>
#include <algorithm>
#include <cctype>


#include "StringUtils.hpp"

// assume input is utf8 as it may come from glib
//  wchar_t is even for 32bit plattform 32bits so we shoud better use utf32 for full compat fontconfig
//    this is the primary use for this function, but use utf16 for windows ?
//static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converterUtf8;
static std::wstring_convert<std::codecvt_utf8<wchar_t>> converterUtf8;
// for c++17 deprecated requires use local with facet, but for our purpose the above works fine


StringUtils::StringUtils()
{
}


StringUtils::~StringUtils()
{
}

std::wstring
StringUtils::from_bytesUtf8(const char *in)
{
    return converterUtf8.from_bytes(in);
}

std::wstring
StringUtils::from_bytesUtf8(const std::string &in)
{
    return converterUtf8.from_bytes(in);
}

void
StringUtils::ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

void
StringUtils::rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

void
StringUtils::trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

std::string
StringUtils::lower(const std::string &str, int start) {
    int i = 0;
    std::string r;
    r.reserve(str.length());
    for (auto s = str.begin(); s != str.end(); ++s)
    {
        if (i >= start)
            r += tolower(*s);
        else
            r += *s;
        ++i;
    }
    return r;
}



const char *
StringUtils::weekday(int day)
{
    switch (day%7)
    {
        case 0:
            return "Sun";
        case 1:
            return "Mon";
        case 2:
            return "Tue";
        case 3:
            return "Wed";
        case 4:
            return "Thu";
        case 5:
            return "Fri";
        case 6:
            return "Sat";
    }
    return "?";
}


void
StringUtils::split(const std::string &line, char delim, std::vector<std::string> &ret)
{
    size_t pos = 0;
    while (pos < line.length()) {
        size_t next = line.find(delim, pos);
        if (next != std::string::npos) {
            std::string fld = line.substr(pos, next - pos);
            ret.push_back(fld);
            ++next;
        }
        else {
            if (pos < line.length()) {
                size_t end = line.length();
                if (line.at(end-1) == '\n') {
                    --end;
                }
                if (end - pos > 0) {
                    std::string fld = line.substr(pos, end - pos);
                    ret.push_back(fld);
                }
            }
            break;
        }
        pos = next;
    }
}

//std::wstring
//StringUtils::toWide(const std::string &name) {
//    size_t len = mbstowcs(nullptr, name.c_str(), 0);
//    if (len == (size_t) -1) {
//        std::wstring sname(L"?");
//        return sname;
//    }
//    std::wstring wname(len, L'#');
//    mbstowcs(&wname[0], name.c_str(), len);
//    return wname;
//}