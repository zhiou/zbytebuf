//
//  normal_test.cpp
//  ZByteBufTests
//
//  Created by zhiou on 2021/8/12.
//  Copyright © 2021 zzstudio. All rights reserved.
//


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

//MARK: 增
TEST_CASE( "The data of zbytebuf cound be increase", "[normal]") {
    
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
            buf.appendIf("ABBC", [](){
                return true;
            });
            REQUIRE(buf.equals("ABBC"));
        }
        
        SECTION ( "append if confition wan't satisfied" ) {
            zbytebuf buf;
            buf.appendIf("0xABBC", [](){
                return false;
            });
            REQUIRE(buf.length() == 0);
        }
    }
    
    if (!rdb) {
        free(rdb);
    }
}


//TODO: 删

//TODO: 改

//TODO: 查

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
    
    REQUIRE(aaa.equals("1122334455112233445511223344551122334455"));
    
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


