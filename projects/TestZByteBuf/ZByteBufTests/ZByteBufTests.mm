//
//  ZByteBufTests.m
//  ZByteBufTests
//
//  Created by zhouhuang on 2019/9/21.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#import <XCTest/XCTest.h>
#import "zbytebuf.hpp"

using namespace zzz;
@interface ZByteBufTests : XCTestCase

@end

@implementation ZByteBufTests

- (void)setUp {
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
}

- (void)testExample {
    // This is an example of a functional test case.
    // Use XCTAssert and related functions to verify your tests produce the correct results.
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
    
    XCTAssert(ret.begin_with("2233"));
    XCTAssert(a == 0x11);
    XCTAssert(b == 0x2233);
    XCTAssert(c == 0x44551122);
    XCTAssert(d == 0x3344551122334455);
    XCTAssert(aaa.hex_str() == "1122334455");
    
    
    zbytebuf bbb("1122334455112233445511223344551122334455");
    size_t offset = 0;
    bbb.scan(a, offset)
    .scan(b, offset)
    .scan(c, offset)
    .scan(d, offset)
    .debug();
    
    XCTAssert(a == 0x11);
    XCTAssert(b == 0x2233);
    XCTAssert(c == 0x44551122);
    XCTAssert(d == 0x3344551122334455);
    
    zbytebuf ccc("1122334455112233445511223344551122334455");
    ccc.replace(1, 2, zbytebuf("1111")).debug();
    
    XCTAssert(ccc == zbytebuf("1111114455112233445511223344551122334455"));
    // sample apdu format: FF AA Len LenR Data CRC
    zbytebuf apdu;
    apdu.append<std::string>("FFAA");
    apdu.append<uint16_t>(0x6AFC);
    apdu.reserve(3).reserve(3);
    apdu += ccc;
    apdu.debug();
    
    XCTAssert(apdu == zbytebuf("ffaa6afc1111114455112233445511223344551122334455"));
    
    apdu.replace(2, 1, zbytebuf("1CB0")).debug();
     XCTAssert(apdu == zbytebuf("ffaa1cb0fc1111114455112233445511223344551122334455"));
    
    apdu.replace(5, 3, zbytebuf("00FF")).debug();
    XCTAssert(apdu == zbytebuf("ffaa1cb0fc00FF4455112233445511223344551122334455"));
    apdu.replace(8, 3, "AABBCCDD").debug();
    apdu.debug();
    XCTAssert(apdu == zbytebuf("ffaa1cb0fc00FF44aabbccdd33445511223344551122334455"));
    apdu.append<uint16_t>(0x645)
    .debug();
    XCTAssert(apdu == zbytebuf("ffaa1cb0fc00FF44aabbccdd334455112233445511223344550645"));
    
    apdu.appendIf<uint16_t>(0xAAAA, [](){
        return true;
    })
    .debug();
     XCTAssert(apdu == zbytebuf("ffaa1cb0fc00FF44aabbccdd334455112233445511223344550645AAAA"));
    
    apdu.appendIf("BBBB", [](){
        return false;
    })
    .debug();
    XCTAssert(apdu == zbytebuf("ffaa1cb0fc00FF44aabbccdd334455112233445511223344550645AAAA"));
    
    apdu.replace(0, 4, zbytebuf("11223344"))
    .debug();
    
    XCTAssert(apdu == zbytebuf("11223344fc00FF44aabbccdd334455112233445511223344550645AAAA"));
    
    apdu.replace(0, 0, zbytebuf("BBBB"))
    .debug();
    
    XCTAssert(apdu == zbytebuf("BBBB11223344fc00FF44aabbccdd334455112233445511223344550645AAAA"));
}

- (void)testPerformanceExample {
    // This is an example of a performance test case.
    [self measureBlock:^{
        // Put the code you want to measure the time of here.
    }];
}

@end
