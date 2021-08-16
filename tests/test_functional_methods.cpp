//
//  test_functional_methods.cpp
//  zbytebuf-utests
//
//  Created by zhiou on 2021/8/14.
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.

#include "catch.hpp"
#include "zbytebuf/zbytebuf.hpp"

using namespace zzz;

TEST_CASE( "zbytebuf functional methods", "[normal]" ) {
    auto size = rand() % 0x800 + 0x10;
    
    zbytebuf buf(size, true);
    
    SECTION( "copy method" ) {
        auto copied = buf;
        REQUIRE(copied == buf.copy());
    }
    
    
    SECTION( "pop method" ) {
        auto copied = buf;
        uint8_t a;
        copied.pop<uint8_t>(a);
        REQUIRE(a == buf[0]);
        
        uint16_t b;
        copied.pop<uint16_t>(b);

        REQUIRE(b == ((buf[1] << 8) | buf[2]));
    
        uint32_t c;
        copied.pop<uint32_t>(c);
        REQUIRE(c == (buf[3] << 24 | buf[4] << 16 | buf[5] << 8 | buf[6]));
    }
    
    SECTION( "scan method" ) {
        size_t offset = 0;
        uint8_t a;
        uint16_t b;
        uint32_t c;
        
        auto copied = buf;
        copied
        .scan(a, offset)
        .scan(b, offset)
        .scan(c, offset);
        
        REQUIRE(a == buf[0]);
        REQUIRE(b == ((buf[1] << 8) | buf[2]));
        REQUIRE(c == (buf[3] << 24 | buf[4] << 16 | buf[5] << 8 | buf[6]));
    }
    
    SECTION( "back methods" ) {
        int n = rand() % size;
        auto back = buf.back(n);
        auto left = buf.copy().drop_back(n);
        REQUIRE(left + back == buf);
    }
    
    SECTION( "front methods" ) {
        int n = rand() % size;
        auto front = buf.front(n);
        auto left = buf.copy().drop_front(n);
        REQUIRE(front + left == buf);
    }
    
    SECTION( "slice method" ) {
        size_t loc = rand() % size;
        size_t len = rand() % (size - loc);
        auto left = buf.copy().slice(loc, len);
        REQUIRE(left == buf.sub_buf(loc, len));
    }
    
    SECTION( "head methods" ) {
        size_t headSize = rand() % 0xFF;
        zbytebuf head(headSize, true);
        auto headed = buf.addHead(head);
        REQUIRE(headed.headWith(head));
    }
    
    SECTION( "pad methods" ) {
        size_t padLen = rand() % 0x10;
        zbytebuf pad(padLen, true);
        buf.pad([&](const zbytebuf& buf) {
            return pad;
        });
        REQUIRE(buf.endWith(pad));
        
        buf.pad_head([&](const zbytebuf& buf) {
            return pad;
        });
        REQUIRE(buf.headWith(pad));
    }
    
    SECTION( "reserve method" ) {
        int n = rand() % size;
        auto reserved = buf.reserve(n);
        REQUIRE(reserved.headWith(buf));
        REQUIRE(reserved.endWith(zbytebuf(n)));
    }
    
    SECTION( "relace method" ) {
        size_t loc = rand() % size;
        size_t len = rand() % (size - loc);
        zbytebuf sub(len, true);
        auto replaced = buf.copy().replace(loc, len, sub);
        
        REQUIRE(buf.sub_buf(0, loc) == replaced.sub_buf(0, loc));
        REQUIRE(buf.sub_buf(loc, len) != replaced.sub_buf(loc, len));
        REQUIRE(buf.sub_buf(loc + len, size - loc - len) == replaced.sub_buf(loc + len, size - loc - len));
        REQUIRE(replaced.sub_buf(loc, len) == sub);
    }
    
    SECTION( "map method" ) {
        auto mapped = buf.copy().map([](const byte& b) {
            return ~b;
        });
        
        REQUIRE(mapped == ~buf);
    }
    
    SECTION( "for_each method" ) {
        zbytebuf other;
        buf.for_each([&](const byte& b) {
            other.append<byte>(b);
        });
        
        REQUIRE(buf == other);
    }
    
    SECTION( "all method" ) {
        zbytebuf other(~buf.copy());
        bool success = buf.all([&](const byte& b){
            return true;
        });
        REQUIRE(success);
        
        bool fail = buf.all([&](const byte& b){
            return false;
        });
        REQUIRE(!fail);
    }
    
    SECTION( "repeat method" ) {
        auto result = buf.copy().repeat(2);
        REQUIRE(result == (buf + buf + buf));
    }
}
