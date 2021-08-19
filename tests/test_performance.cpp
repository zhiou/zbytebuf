//
//  test_performance.cpp
//  zbytebuf-utests
//
//  Created by zhiou on 2021/8/19.
//

#include "catch.hpp"
#include "zbytebuf/zbytebuf.hpp"


TEST_CASE("performance test", "[perf]")
{
    
    BENCHMARK_ADVANCED("100B random buf constructor")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure([&](int run) {
            zzz::zbytebuf(100, true);
        });
    };
    
    BENCHMARK_ADVANCED("100B random buf factory create")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure([&](int run) {
            zzz::zbytebuf::rand(100);
        });
    };
    
    BENCHMARK_ADVANCED("100B buf assignment")(Catch::Benchmark::Chronometer meter)
    {
        zzz::zbytebuf buf(100, true);
        meter.measure([&](int run) {
            auto assigned = buf;
        });
    };
    
    BENCHMARK_ADVANCED("100B buf copy")(Catch::Benchmark::Chronometer meter)
    {
        zzz::zbytebuf buf(100, true);
        meter.measure([&](int run) {
            buf.copy();
        });
    };
    
    BENCHMARK_ADVANCED("100B buf assignment")(Catch::Benchmark::Chronometer meter)
    {
        zzz::zbytebuf buf(100, true);
        meter.measure([&](int run) {
            auto assigned = buf;
        });
    };
    
    BENCHMARK_ADVANCED("100B buf append")(Catch::Benchmark::Chronometer meter)
    {
        zzz::zbytebuf buf(100, true);
        auto assigned = buf;
        meter.measure([&](int run) {
            buf.push_back(assigned);
        });
    };
    
    BENCHMARK_ADVANCED("100B buf sub")(Catch::Benchmark::Chronometer meter)
    {
        zzz::zbytebuf buf(100, true);
  
        meter.measure([&](int run) {
            buf.sub_buf(0, 100);
        });
    };
    
    BENCHMARK_ADVANCED("100B buf equals")(Catch::Benchmark::Chronometer meter)
    {
        zzz::zbytebuf buf(100, true);
        auto assigned = buf;
        meter.measure([&](int run) {
            bool equal = (buf == assigned);
        });
    };
    
    BENCHMARK_ADVANCED("100B hex string equals")(Catch::Benchmark::Chronometer meter)
    {
        zzz::zbytebuf buf(100, true);
        std::string hexstr = buf.hex_str();
        meter.measure([&](int run) {
            bool equal = (buf == hexstr);
        });
    };
}
