//
//  main.cpp
//  TestZByteBuf
//
//  Created by zhouhuang on 2019/9/20.
//  Copyright © 2019 zzstudio. All rights reserved.
//

//#include <iostream>
#include "zbytebuf.hpp"

using namespace zzstudio;

zbytebuf add(zbytebuf& a, zbytebuf b)
{
    return a += b;
}

zbytebuf crc(const zbytebuf& ori)
{
    return "EEFF";
}

int main(int argc, const char * argv[]) {
    // insert code here...
    zbytebuf bb("1122334455667788");
    std::cout << bb.hex_str() << std::endl;
    std::cout << bb << std::endl;
    
    auto a = bb.read<unsigned short>(0);
    auto b = bb.read<uint32_t>(0);
    auto c = bb.read<uint64_t>(0);
    auto d = bb[0];
    
    add(bb, "112233");
    
    bb.append(d);
    bb.append(b);
    bb.append(a);
    bb.append(d);
    std::cout << bb << std::endl;
    
    bb.pad([](const zbytebuf& ori){
        return "FFFF";
    });
    bb.pad_head(crc);
    std::cout << bb << std::endl;
    return 0;
}
