//
//  test_performance.cpp
//  zbytebuf-utests
//
//  Created by zhiou on 2021/8/19.
//

#include "catch.hpp"
#include "zbytebuf/zbytebuf.hpp"

#if defined(CATCH_CONFIG_ENABLE_BENCHMARKING)

TEST_CASE("performance test", "[perf]")
{
    BENCHMARK_ADVANCED("100B random buf constructor")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<Catch::Benchmark::storage_for<zzz::zbytebuf>> storage(meter.runs());
        meter.measure([&](int run) {
            return storage[run].construct(100, true);
        });
    };
    
    BENCHMARK_ADVANCED("100B random buf destructor")(Catch::Benchmark::Chronometer meter)
    {
        std::vector<Catch::Benchmark::destructable_object<zzz::zbytebuf>> storage(meter.runs());
        for(auto&& o : storage)
            o.construct(100);
        meter.measure([&](int run) {
            return storage[run].destruct();
        });
    };
    
    BENCHMARK_ADVANCED("100B random buf factory create")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure([&](int run) {
            return zzz::zbytebuf::rand(100);
        });
    };
    
    BENCHMARK_ADVANCED("100B buf assignment")(Catch::Benchmark::Chronometer meter)
    {
        zzz::zbytebuf buf(100, true);
        meter.measure([&](int run) {
            auto assigned = buf;
            return assigned;
        });
    };
    
    BENCHMARK_ADVANCED("100B buf copy")(Catch::Benchmark::Chronometer meter)
    {
        zzz::zbytebuf buf(100, true);
        meter.measure([&](int run) {
            return buf.copy();
        });
    };
    
    BENCHMARK_ADVANCED("100B buf assignment")(Catch::Benchmark::Chronometer meter)
    {
        zzz::zbytebuf buf(100, true);
        meter.measure([&](int run) {
            auto assigned = buf;
            return assigned;
        });
    };
    
    BENCHMARK_ADVANCED("100B buf append")(Catch::Benchmark::Chronometer meter)
    {
        zzz::zbytebuf buf(100, true);
        auto assigned = buf;
        meter.measure([&](int run) {
            return buf.push_back(assigned);
        });
    };
    
    BENCHMARK_ADVANCED("100B buf sub")(Catch::Benchmark::Chronometer meter)
    {
        zzz::zbytebuf buf(100, true);
  
        meter.measure([&](int run) {
            return buf.sub_buf(0, 100);
        });
    };
    
    BENCHMARK_ADVANCED("100B buf equals")(Catch::Benchmark::Chronometer meter)
    {
        zzz::zbytebuf buf(100, true);
        auto assigned = buf;
        meter.measure([&](int run) {
            return (buf == assigned);
        });
    };
    
    BENCHMARK_ADVANCED("100B hex string equals")(Catch::Benchmark::Chronometer meter)
    {
        zzz::zbytebuf buf(100, true);
        std::string hexstr = buf.hex_str();
        meter.measure([&](int run) {
            return  (buf == hexstr);
        });
    };
}

#endif
