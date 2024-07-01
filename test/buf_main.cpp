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


#include <iostream>
#include <cstdlib>
#include <algorithm> //if we getting std::copy to work
#include <iterator>

#include "Buffer.hpp"


static bool
bufferTest()
{
    std::cout << "buffer --------------" << std::endl;
    const auto size{100u};
    const auto exp_sum{size * (size + 1) / 2};
    Buffer<uint64_t> buf(size);
    for (uint32_t i = 0u; i < buf.getSize(); ++i) {
        buf.roll();
        buf.set(i+1);
    }
    std::cout << "sum " << buf.sum() << std::endl;
    if (buf.sum() != exp_sum) {
        std::cout << "Not the expected sum ! expected " << exp_sum << std::endl;
        return false;
    }
    buf.roll();
    for (uint32_t i = 0u; i < buf.getSize(); ++i) {
        uint64_t exp = i+2;
        if (i == buf.getSize()-1) {
            exp = 0u;
        }
        if (buf.get(i) != exp) {
            std::cout << "Not the expected value "<< buf.get(i) << " expected " << exp << std::endl;
            return false;
        }
    }
    std::cout << "buffer --------------" << std::endl;
    return true;
}


int
main(int argc, char** argv) {
    if (!bufferTest()) {
        return 1;
    }

    return 0;
}

