//
//  normal_test.cpp
//  ZByteBufTests
//
//  Created by zhiou on 2021/8/12.
//  Copyright © 2021 zzstudio. All rights reserved.
//


#include "catch.hpp"
#include "zbytebuf.hpp"

using namespace zzz;

unsigned int Factorial( unsigned int number ) {
    return number <= 1 ? number : Factorial(number-1)*number;
}

TEST_CASE( "Factorials are computed", "[factorial]" ) {
    REQUIRE( Factorial(1) == 1 );
    REQUIRE( Factorial(2) == 2 );
    REQUIRE( Factorial(3) == 6 );
    REQUIRE( Factorial(10) == 3628800 );
}

TEST_CASE( "zbytebuf normal test", "[normal]" ) {
    zbytebuf zbb("1122334455");
    
    auto ret = zbb
    .map([](const byte& b){
        return b;
    })
    .for_each([](const byte& b){
        
    })
    .drop_back(1)
    .drop_front(1)
    .debug();
    
    zbytebuf aaa("1122334455112233445511223344551122334455");
    uint8_t a;
    uint16_t b;
    uint32_t c;
    uint64_t d;
    
    aaa
    .pop(a)
    .pop(b)
    .pop(c)
    .pop(d)
    .debug();
    
    REQUIRE(ret.begin_with("2233"));
    REQUIRE(a == 0x11);
    REQUIRE(b == 0x2233);
    REQUIRE(c == 0x44551122);
    REQUIRE(d == 0x3344551122334455);
    REQUIRE(aaa.hex_str() == "1122334455");
    
    
    zbytebuf bbb("1122334455112233445511223344551122334455");
    size_t offset = 0;
    bbb.scan(a, offset)
    .scan(b, offset)
    .scan(c, offset)
    .scan(d, offset)
    .debug();
    
    REQUIRE(a == 0x11);
    REQUIRE(b == 0x2233);
    REQUIRE(c == 0x44551122);
    REQUIRE(d == 0x3344551122334455);
    
    zbytebuf ccc("1122334455112233445511223344551122334455");
    ccc.replace(1, 2, zbytebuf("1111")).debug();
    
    REQUIRE(ccc == zbytebuf("1111114455112233445511223344551122334455"));
    // sample apdu format: FF AA Len LenR Data CRC
    zbytebuf apdu;
    apdu.append<std::string>("FFAA");
    apdu.append<uint16_t>(0x6AFC);
    apdu.reserve(3).reserve(3);
    apdu += ccc;
    apdu.debug();
    
    REQUIRE(apdu == zbytebuf("ffaa6afc1111114455112233445511223344551122334455"));
    
    apdu.replace(2, 1, zbytebuf("1CB0")).debug();
     REQUIRE(apdu == zbytebuf("ffaa1cb0fc1111114455112233445511223344551122334455"));
    
    apdu.replace(5, 3, zbytebuf("00FF")).debug();
    REQUIRE(apdu == zbytebuf("ffaa1cb0fc00FF4455112233445511223344551122334455"));
    apdu.replace(8, 3, "AABBCCDD").debug();
    apdu.debug();
    REQUIRE(apdu == zbytebuf("ffaa1cb0fc00FF44aabbccdd33445511223344551122334455"));
    apdu.append<uint16_t>(0x645)
    .debug();
    REQUIRE(apdu == zbytebuf("ffaa1cb0fc00FF44aabbccdd334455112233445511223344550645"));
    
    apdu.appendIf<uint16_t>(0xAAAA, [](){
        return true;
    })
    .debug();
     REQUIRE(apdu == zbytebuf("ffaa1cb0fc00FF44aabbccdd334455112233445511223344550645AAAA"));
    
    apdu.appendIf("BBBB", [](){
        return false;
    })
    .debug();
    REQUIRE(apdu == zbytebuf("ffaa1cb0fc00FF44aabbccdd334455112233445511223344550645AAAA"));
    
    apdu.replace(0, 4, zbytebuf("11223344"))
    .debug();
    
    REQUIRE(apdu == zbytebuf("11223344fc00FF44aabbccdd334455112233445511223344550645AAAA"));
    
    apdu.replace(0, 0, zbytebuf("BBBB"))
    .debug();
    
    REQUIRE(apdu == zbytebuf("BBBB11223344fc00FF44aabbccdd334455112233445511223344550645AAAA"));
}


