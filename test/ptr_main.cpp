/* -*- Mode: c++; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4; coding: utf-8; -*-  */
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
#include <deque>
#include <source_location>
#include <ConcurrentCollections.hpp>


std::ostream&
operator<< (std::ostream& os, const std::source_location& location)
{
    os << location.file_name() << "("
       << location.line() << ":"
       << location.column() << ") "
       << location.function_name() << ";";
    return os;
}

// a additional test should be to run "valgrind ptr_test"

#include "ptr_main.hpp"
#include "active_ptr.hpp"


static int baseFreed{0};
static int testFreed{0};

Base::Base(int n)
: m_val{n}
{
    ++baseFreed;
}

Base::~Base()
{
    --baseFreed;
    std::cout << std::source_location::current() << " val " << m_val << std::endl;
}

void
Base::dummy()
{
    std::cout << std::source_location::current() << std::endl;
}

void
Base::base()
{
    std::cout << std::source_location::current() << std::endl;
}

int Base::get()
{
    return m_val;
}

Test::Test(int n)
: Base(n)
{
    std::cout << std::source_location::current() << " val " << m_val << std::endl;
    ++testFreed;
}

Test::~Test()
{
    --testFreed;
    std::cout << std::source_location::current() << " val " << m_val << std::endl;
}

void
Test::dummy()
{
    std::cout << std::source_location::current() << std::endl;
}

void
Test::test()
{
    std::cout << std::source_location::current() << " val " << m_val << std::endl;
    ++m_val;
}

static void
funct(const psc::mem::active_ptr<Base>& base2) noexcept
{
    std::cout << "funct base " << base2 << std::endl;
    auto baseLease = base2.lease();
    if (baseLease) {
        baseLease->dummy();
        //std::cout << baseLease->get() << std::endl;
    }
    else {
        std::cout << "funct base no access"  << std::endl;
    }
}

static bool
simpleTest()
{
    baseFreed = 0;
    testFreed = 0;
    std::cout << "simple --------------" << std::endl;
    {
        auto test = psc::mem::make_active<Test>(7);
        test = nullptr;
    }
    std::cout << "simple --------------"
              << " base " << baseFreed
              << " test " << testFreed
              << std::endl;

    return baseFreed == 0 && testFreed == 0;
}

// this rather expects to look on the output
static bool
ptrTest()
{
    baseFreed = 0;
    testFreed = 0;

    std::cout << "ptr --------------" << std::endl;
    //Test* ttest = new Test(8);
    // this will be 4 bytes higher than m_val
    //std::cout << "ttest " << std::hex << &ttest->m_n << std::dec << std::endl;
    //Base* tbase = dynamic_cast<Base*>(ttest);
    //std::cout << "tbase " << std::hex << &tbase->m_val << std::dec << std::endl;
    //std::cout << "----------------" << std::endl;
    {
        psc::mem::active_ptr<Test> test;
        {
            std::cout << "---- before make_active " << (test ? "valid" : "invalid") << std::endl;
            test = psc::mem::make_active<Test>(7);
            std::cout << "---- after make_active " << test << std::endl;
            {
                auto testLease = test.lease();
                if (testLease) {
                    testLease->test();
                    std::cout << "test get " << testLease->get() << std::endl;
                }
            }
            {
                funct(test);
                {
                    std::cout << "-- using copy cc " << std::endl;
                    auto test2{test};  // will use implicite copy
                    std::cout << "-- after 2inst " << test << std::endl;
                    //test2 = test; uses assignment
                    {
                        auto lease1 = test2.lease();
                        if (lease1) {
                            std::cout << "test2 get " << lease1 << std::endl;
                            lease1->test();
                            test2.resetAll();
                            lease1->test();
                        }
                        else {
                            std::cout << "No access lease1 " << std::endl;
                        }
                    }
                }
                std::cout << "-- after test2 decayed  " << test << std::endl;
                {
                    auto lease2 = test.lease();
                    if (lease2) {
                        lease2->test();
                    }
                    else {
                        std::cout << "No access lease2" << std::endl;
                    }
                }
                std::cout << "-- closing test2" << test << std::endl;
            }
        }
        std::cout << "-- testing valid " << (test ? "valid" : "invalid") << std::endl;
    }
    std::cout << "ptr --------------"
              << " base " << baseFreed
              << " test " << testFreed
              << std::endl;

    return baseFreed == 0 && testFreed == 0;
}



static bool
releaseTest()
{
    baseFreed = 0;
    testFreed = 0;
    std::cout << "release -----------" << std::endl;
    {
        auto active_ptr1 = psc::mem::make_active<Test>(8);
        std::cout << "ptr1 " << std::hex << active_ptr1.get() << std::endl;
        // default const.  addr 0 use 0 lease 0
        // ptr_main.cpp(65:47) Test::Test(int); n 8
        // ptr1 0x5b12ce31e310
        auto active_ptr2 = psc::mem::make_active<Test>(9);
        std::cout << "ptr2 " << std::hex << active_ptr2.get() << std::endl;
        // default const.  addr 0 use 0 lease 0
        // ptr_main.cpp(65:47) Test::Test(int); n 9
        // ptr2 0x5b12ce31e3d0
        auto lease1 = active_ptr1.lease();
        if (lease1) {
            active_ptr1 = active_ptr2;
            // active_ptr::removeControlIfUnsued
            // active_ptr assign lval  addr 0x5600ef2d53d0 use 1 lease 0
            lease1->test();
            // ptr_main.cpp(83:47) void Test::test(); n 8
        }
    }
    //~active_lease addr 0x5fc53cdc6310 lease 1
    //active_ctl::reset dealloc addr 0x5fc53cdc6310 use 0 lease 0
    //ptr_main.cpp(73:47) virtual Test::~Test(); n 9
    //ptr_main.cpp(48:47) virtual Base::~Base(); val 0
    //active_lease::removeControlIfUnsued delete addr 0x5fc53cdc6310 lease 0
    //~active_ctl addr 0 use 0 lease 0
    //~active_ptr  addr 0x5fc53cdc63d0 use 2 lease 0
    //active_ctl::reset dealloc addr 0x5fc53cdc63d0 use 1 lease 0
    //ptr_main.cpp(73:47) virtual Test::~Test(); n 9
    //ptr_main.cpp(48:47) virtual Base::~Base(); val 0
    //active_ptr::removeControlIfUnsued
    //~active_ptr  addr 0 use 1 lease 0
    //active_ctl::reset dealloc addr 0 use 0 lease 0
    //active_ptr::removeControlIfUnsued
    //~active_ctl addr 0 use 0 lease 0
    std::cout << "release ---------- "
              << " base " << baseFreed
              << " test " << testFreed
              << std::endl;
    return baseFreed == 0&& testFreed == 0;
}

static bool
coversionTest()
{
    baseFreed = 0;
    testFreed = 0;
    std::cout << "conversion ---------- " << std::endl;
    {
        psc::mem::active_ptr<Base> base;
        auto active_ptr1 = psc::mem::make_active<Test>(11);
        // psc::mem::active_ptr<Unrelated> unrelated{base};  //this should not compile ...
        auto unrelated = psc::mem::dynamic_pointer_cast<Unrelated>(base);
        if (unrelated) {    // if this becomes active there is something wrong
            return false;
        }
        auto lemty = unrelated.lease(); // check access to "empty" ptr
        if (lemty) {
            return false;
        }
        base = active_ptr1;     // implicit conversion
        active_ptr1.resetThis();
        if (auto lemty2 = base.lease()) {
            base.resetThis();
            lemty2->dummy(); // still usable?
        }
        else {
            std::cout << "second 2 no access" << std::endl;
            return false;
        }
    }
    std::cout << "conversion ---------- "
              << " base " << baseFreed
              << " test " << testFreed
              << std::endl;
    return baseFreed == 0 && testFreed == 0;
}

static bool
compareTest()
{
    baseFreed = 0;
    testFreed = 0;
    std::cout << "compare ---------- " << std::endl;
    {
        psc::mem::active_ptr<Base> base;
        if (base != nullptr) {    // this might happend while migrating, and should still work
            std::cout << "base and nullptr are not equal!" << std::endl;
            return false;
        }
        auto active_ptr1 = psc::mem::make_active<Test>(33);
        auto unrelated = psc::mem::dynamic_pointer_cast<Unrelated>(base);
        if (unrelated != nullptr) {    // if this becomes active there is something wrong
            std::cout << "unrealted and nullptr are not equal!" << std::endl;
            return false;
        }
        base = active_ptr1;     // implicit conversion
        if (base != active_ptr1) {
            std::cout << "base and active not equal!" << std::endl;
            return false;
        }
        base.resetThis();
        if (base == active_ptr1) {
            std::cout << "after reset base and active are equal!" << std::endl;
            return false;
        }
    }
    std::cout << "compare ---------- "
              << " base " << baseFreed
              << " test " << testFreed
              << std::endl;
    return baseFreed == 0 && testFreed == 0;

}

static psc::mem::active_ptr<Base>
fun()
{
    return nullptr;
}

static bool
creationTest()
{
    baseFreed = 0;
    testFreed = 0;
    std::cout << "creation -----------" << std::endl;
    psc::mem::active_ptr<Base> base{nullptr};
    std::cout << "use " << base << std::endl;
    if (base) {
        return false;
    }
    if (auto lbase = base.lease()) {
        return false;
    }
    base = fun();
    std::cout << "use " << base << std::endl;
    if (base) {
        return false;
    }
    if (auto lbase = base.lease()) {
        return false;
    }
    std::cout << "creation ---------- "
              << " base " << baseFreed
              << " test " << testFreed
              << std::endl;

    return baseFreed == 0 && testFreed == 0;
}

static bool
sharedTest()
{
    baseFreed = 0;
    testFreed = 0;
    // try some shared_ptr behaviour
    std::cout << "shared -----------" << std::endl;
    {
        std::vector<std::shared_ptr<Test>> coll;
        auto shr = std::make_shared<Test>(88);
        std::cout << "shr use count " << shr.use_count() << std::endl;
        coll.emplace_back(std::move(shr));
        std::cout << "shr use count " << shr.use_count() << std::endl;  // as it seems
        for (auto& back : coll) {
            //coll.pop_back();
            std::cout << "shr use count " << back.use_count() << std::endl;
        }
        std::vector<psc::mem::active_ptr<Test>> coll2;
        auto aut = psc::mem::make_active<Test>(89);
        std::cout << "act use count " << aut.use_count() << std::endl;
        coll2.emplace_back(std::move(aut));
        std::cout << "act use count " << aut.use_count() << std::endl;  // if collection is the same type as ptr, rval assigment is used :)
        for (auto& back : coll2) {
            std::cout << "act use count " << back.use_count() << std::endl;
        }
    }
    std::cout << "shared -----------"
              << " base " << baseFreed
              << " test " << testFreed
              << std::endl;

    return baseFreed == 0 && testFreed == 0;
}

static bool
collectionTest()
{
    baseFreed = 0;
    testFreed = 0;
    std::cout << "collection -----------" << std::endl;
    if (true) {     // as we run into trouble with this test this as well
        TQueueConcurrent<psc::mem::active_ptr<Base>> thumbnailReaderQueue;
        auto entry = psc::mem::make_active<Test>(99);
        std::cout << "collcc emplace_back --------" << std::endl;
        thumbnailReaderQueue.emplace_back(std::move(entry));
        std::cout << "collcc pop_front -----" << std::endl;
        auto front = thumbnailReaderQueue.pop_front();
        std::cout << "collcc poped_front -----" << std::endl;
        if (!front) {
            return false;
        }
        if (auto lfront = front.lease()) {
            std::cout << "val " << lfront->get() << std::endl;
        }
    }
    std::cout << "collection ---------- "
              << " base " << baseFreed
              << " test " << testFreed
              << std::endl;

    return baseFreed == 0 && testFreed == 0;
}

static bool
livespanTest()
{
    baseFreed = 0;
    testFreed = 0;
    std::cout << "livespan -----------" << std::endl;
    psc::mem::active_ptr<Base> base;
    // active_ptr default const.  addr 0 use 0 lease 0
    {
        auto test = psc::mem::make_active<Test>(7);
        //active_ptr default const.  addr 0 use 0 lease 0
        //ptr_main.cpp(68:47) Test::Test(int); n 7
        base = test;
        // active_ptr conversion  addr 0x594e91e94360 use 1 lease 0
        // active_ctl::reset dealloc addr 0 use 0 lease 0
        // active_ptr::removeControlIfUnsued
        // ~active_ctl addr 0 use 0 lease 0
        // active_ptr assign rval  addr 0x594e91e94360 use 2 lease 0
        // ~active_ptr  addr 0 use 0 lease 0
        // active_ptr::removeControlIfUnsued
        // ~active_ptr  addr 0x594e91e94360 use 2 lease 0
        // active_ptr::removeControlIfUnsued
    }
    std::cout << "use " << base.use_count()
              << " state " << (base ? "" : "in") << "active"
              << " adr " << std::hex << base.get() << std::dec
              << std::endl;
    // use 1 state active adr 0x5ea83084a360
    base.resetAll();
    // active_ctl::reset dealloc addr 0x5ea83084a360 use 1 lease 0
    // ptr_main.cpp(75:47) virtual Test::~Test(); n 7
    // ptr_main.cpp(49:47) virtual Base::~Base(); val 0
    // check Test destructor is called ...
    std::cout << "use " << base.use_count()
              << " state " << (base ? "" : "in") << "active"
              << " adr " << std::hex << base.get() << std::dec
              << std::endl;
    // use 2 state inactive adr 0
    std::cout << "livespan ---------- "
              << " base " << baseFreed
              << " test " << testFreed
              << std::endl;

    return baseFreed == 0 && testFreed == 0;
}

int
main(int argc, char** argv) {
    psc::mem::active_debug = false; // if needed change active_debug to no const
    if (!simpleTest()) {
        return 1;
    }
    if (!ptrTest()) {
        return 2;
    }
    if (!coversionTest()) {
        return 3;
    }
    if (!livespanTest()) {
        return 4;
    }
    if (!releaseTest()) {
        return 5;
    }
    if (!creationTest()) {
        return 6;
    }
    if (!collectionTest()) {
        return 7;
    }
    if (!compareTest()) {
        return 8;
    }
    if (!sharedTest()) {
        return 9;
    }

    return 0;
}
