//
//  main.cpp
//  TestZByteBuf
//
//  Created by zhouhuang on 2019/9/20.
//  Copyright © 2019 zzstudio. All rights reserved.
//

#include <iostream>
#include "zbytebuf.hpp"

using namespace zzstudio;

int main(int argc, const char * argv[]) {
    // insert code here...
    zbytebuf bb("1122334455667788");
    std::cout << bb.hex_str() << std::endl;
    std::cout << bb << std::endl;
    
    auto a = bb.read<uint16_t>(0);
    auto b = bb.read<uint32_t>(0);
    auto c = bb.read<uint64_t>(0);
    return 0;
}
