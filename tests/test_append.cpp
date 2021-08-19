//
//  normal_test.cpp
//  ZByteBufTests
//
//  Created by zhiou on 2021/8/12.
//  Copyright © 2021 zzstudio. All rights reserved.
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

std::string bytes2string(byte* bytes, size_t len) {
    std::stringstream ss;
    for (int i = 0; i < len; i++) {
        ss << std::uppercase << std::setw(2) << std::setfill('0') << std::hex
        << (unsigned int)bytes[i];
    }
    return ss.str();
}


//MARK: 增
TEST_CASE( "zbytebuf data increased", "[normal]") {
    
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
    
    
    SECTION ("append uint_8") {
        zbytebuf buf;
        buf.append<uint8_t>(0x6A);
        REQUIRE(buf.equals("6A"));
    }
    
    SECTION ("append uint_16") {
        zbytebuf buf;
        buf.append<uint16_t>(0x6AFC);
        REQUIRE(buf.equals("6AFC"));
    }
    
    SECTION ("append uint_32") {
        zbytebuf buf;
        buf.append<uint32_t>(0x6AFCABCE);
        REQUIRE(buf.equals("6AFCABCE"));
    }
    
    SECTION ("append uint_64") {
        zbytebuf buf;
        buf.append<uint64_t>(0x6AFCABCEFFAA1122);
        REQUIRE(buf.equals("6AFCABCEFFAA1122"));
    }
    
    SECTION ("append no integer value") {
        
        SECTION ("append hex string") {
            zbytebuf buf;
            buf.append<std::string>(rds);
            REQUIRE(buf.hex_str() == rds);
        }
        
        SECTION ("append vector") {
            zbytebuf buf;
            buf.append<byte_v>(vec);
            REQUIRE(buf.mem() == vec);
        }
    }
    
    SECTION ("append no integer value with specified length 10") {
        
        SECTION ("append hex string") {
            zbytebuf buf;
            buf.append<std::string, 10>(rds);
            REQUIRE(buf.length() == 10);
        }
        
        SECTION ("append vector") {
            zbytebuf buf;
            buf.append<byte_v, 10>(vec);
            REQUIRE(buf.length() == 10);
        }
    }
    
    SECTION( "append if" ) {
        SECTION ( "append if confition was satisfied" ) {
            zbytebuf buf;
            buf.append_if("ABBC", [](){
                return true;
            });
            REQUIRE(buf.equals("ABBC"));
        }
        
        SECTION ( "append if confition wan't satisfied" ) {
            zbytebuf buf;
            buf.append_if("0xABBC", [](){
                return false;
            });
            REQUIRE(buf.length() == 0);
        }
    }
    
    if (!rdb) {
        free(rdb);
    }
}
