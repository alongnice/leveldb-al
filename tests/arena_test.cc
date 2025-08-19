#include <gtest/gtest.h>

#include "arena.h"
#include "random.h"

namespace leveldb {
TEST(ArenaTest, Empty) { Arena arena;}

TEST(ArentTest, Simple){
    std::vector<std::pair<size_t, char*>> allocated;
    Arena arena;
    const int N = 1e5;
    size_t bytes = 0;
    Random rnd(301);
    for(int i = 0; i < N; ++i){
        size_t s;
        if(i%(N/10)==0) s = i;
        else {
            s = rnd.OneIn(4000)?rnd.Uniform(6000):(rnd.OneIn(10)?rnd.Uniform(100):rnd.Uniform(20));
        }

        if(s==0) s = 1;
        // 打印每次分配的字节数
        if (s > 1024 || i % 1000 == 0) {
            printf("alloc[%d]: %zu\n", i, s);
        }
        char* r;
        if(rnd.OneIn(10)) r = arena.AllocateAligned(s);
        else r = arena.Allocate(s);

        for(size_t b = 0; b < s; ++b) r[b] = i % 256;
        bytes += s;
        allocated.emplace_back(std::make_pair(s,r));
        // allocated.emplace_back(s,r);
        ASSERT_GE(arena.MemoryUsage(), bytes);
        if( i > N/10 ) ASSERT_LE(arena.MemoryUsage(), bytes * 1.5);
    }
    for(size_t i=0; i<allocated.size(); ++i){
        size_t num_bytes = allocated[i].first;
        const char* p = allocated[i].second;
        for(size_t b = 0; b < num_bytes; ++b) {
            ASSERT_EQ(int(p[b]) & 0xff, i % 256);
        }
    }
}

}
