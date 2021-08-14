//
//  test_math_operators.cpp
//  zbytebuf-utests
//
//  Created by zhiou on 2021/8/14.
//


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
