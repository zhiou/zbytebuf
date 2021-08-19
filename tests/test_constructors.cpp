//
//  test_constructors.cpp
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

extern std::string bytes2string(byte* bytes, size_t len);

TEST_CASE( "zbytebuf constructors", "[normal]" ) {
    auto size = GENERATE(take(10, random(0, 0x800)));
    
    byte* rdb = (byte*)malloc(size);
    for (int i = 0; i < size; i++) {
        rdb[i] = rand() % 0xFF;
    }
    std::string rds = bytes2string(rdb, size);
    auto vec = byte_v();
    for (int i = 0; i < size; i++) {
        vec.push_back(rand() % 0xFF);
    }
    
    SECTION( "create without parameter") {
        REQUIRE( zbytebuf().length() == 0 );
    }
    
    SECTION( "create with size") {
        
        zbytebuf buf(size);
        REQUIRE( buf.length() == size);
        REQUIRE( buf.all([](const byte& b){
            return b == 0x00;
        }));
    }
    
    SECTION( "create with specified size and random data" ) {
        zbytebuf buf(size, true);
        REQUIRE( buf.length() == size);
        REQUIRE( !buf.all([](const byte& b){
            return b == 0x00;
        }));
    }
    
    SECTION( "create with hex bytes" ) {
        zbytebuf buf(rdb, size);
        REQUIRE( buf.length() == size);
        REQUIRE( memcmp(buf.bytes(), rdb, size) == 0);
    }
    
    SECTION( "create with hex string" ) {
        zbytebuf buf(rds);
        REQUIRE( buf.length() == size);
        REQUIRE( buf.hex_str() == rds);
    }
    
    SECTION( "create with vector" ) {
        SECTION( "create with lvalue" ) {
            zbytebuf buf(vec);
            REQUIRE(buf.mem() == vec);
        }
        
        SECTION( "create with rvalue" ) {
            zbytebuf buf((byte_v(vec)));
            REQUIRE(buf.mem() == vec);
        }
    }
    
    SECTION( "copy from another zbytebuf" ) {
        SECTION( "copy from lvalue" ) {
            zbytebuf buf(size, true);
            auto copied(buf);
            REQUIRE(copied == buf);
        }
        
        SECTION( "copy from rvalue" ) {
            zbytebuf copied((zbytebuf(rds)));
            REQUIRE(copied.hex_str() == rds);
        }
    }
    
    SECTION( "assign from another zbytebuf" ) {
        SECTION( "assign from lvalue" ) {
            auto buf = zbytebuf(size, true);
            auto copied = buf;
            REQUIRE(copied == buf);
        }
        
        SECTION( "assign from rvalue" ) {
            zbytebuf copied = zbytebuf(rds);
            REQUIRE(copied.hex_str() == rds);
        }
    }
    
    SECTION( "create by factory methods" ) {
        auto rand = zbytebuf::rand(10);
        REQUIRE(rand.length() == 10);
        
        auto empty = zbytebuf::empty();
        REQUIRE(empty.length() == 0);
        
        auto nLen = zbytebuf::alloc(10);
        REQUIRE(nLen.length() == 10);
        
    }
    
    if (rdb) {
        free(rdb);
    }
    
}
