#include <gtest/gtest.h>
#include "../include/leveldb/comparator.h"
#include "../include/leveldb/slice.h"

namespace leveldb {

TEST(ComparatorTest, Compare) {
    const Comparator* cmp = BytewiseComparator();
    Slice a("abc");
    Slice b("abd");
    EXPECT_LT(cmp->Compare(a, b), 0);
    EXPECT_GT(cmp->Compare(b, a), 0);
    EXPECT_EQ(cmp->Compare(a, a), 0);
}

TEST(ComparatorTest, Name) {
    const Comparator* cmp = BytewiseComparator();
    EXPECT_STREQ(cmp->Name(), "leveldb.BytewiseComparator");
}

TEST(ComparatorTest, FindShortestSeparator) {
    const Comparator* cmp = BytewiseComparator();
    std::string start = "apple";
    Slice limit("banana");
    cmp->FindShortestSeparator(&start, limit);
    EXPECT_LT(cmp->Compare(Slice(start), limit), 0);
    EXPECT_GE(cmp->Compare(Slice(start), Slice("apple")), 0);
}

TEST(ComparatorTest, FindShortSuccessor) {
    const Comparator* cmp = BytewiseComparator();
    std::string key = "abc";
    cmp->FindShortSuccessor(&key);
    EXPECT_GE(cmp->Compare(Slice(key), Slice("abc")), 0);
}

} // namespace leveldb