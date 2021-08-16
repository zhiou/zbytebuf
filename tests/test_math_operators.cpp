//
//  test_math_operators.cpp
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

TEST_CASE( "zbytebuf math operators", "[normal]" ) {
    auto size = rand() % 0x800 + 1;
    
    zbytebuf buf(size, true);
    
    SECTION( "script operaters" ) {
        byte rb = rand() % 0xFF;
        int index = rand() % size;
        buf[index] = rb;
        REQUIRE(buf[index] == rb);
    }
    
    
    SECTION( "compare operators" ) {
        zbytebuf diff(size);
        zbytebuf same(buf);
        REQUIRE(buf != diff);
        REQUIRE(buf != diff.hex_str());
        REQUIRE(buf == same);
        REQUIRE(buf == same.hex_str());
        REQUIRE(buf.equals(same));
    }
    
    SECTION( "add operators" ) {
        zbytebuf other(size, true);
        
        REQUIRE((buf += other) == buf.append<zbytebuf>(other));
        REQUIRE((buf + other) == buf.append<zbytebuf>(other));
    }
    
    SECTION( "~ operator" ) {
        zbytebuf notBuf(buf);
        REQUIRE(~notBuf == ~buf);
    }
    
    SECTION( "^ operaters") {
        zbytebuf other(buf);
        REQUIRE((buf ^ other) == (buf ^= other));
    }
}
