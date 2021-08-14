//
//  test_constructors.cpp
//  zbytebuf-utests
//
//  Created by zhiou on 2021/8/14.
//

#include "catch.hpp"
#include "zbytebuf/zbytebuf.hpp"

using namespace zzz;

extern std::string bytes2string(byte* bytes, size_t len);

TEST_CASE( "zbytebuf constructors", "[normal]" ) {
    auto size = rand() % 0x800;
    
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
    
    if (rdb) {
        free(rdb);
    }
    
}
