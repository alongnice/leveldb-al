#include "../../include/leveldb/comparator.h"
#include "../../include/leveldb/slice.h"

#include <cstdint>

namespace leveldb {

Comparator::~Comparator() = default;
namespace {
/**
 * @brief 字典序比较器实现
 */
class BytewiseComparatorImpl : public Comparator {
public:
    BytewiseComparatorImpl() = default;
    const char* Name() const override { return "leveldb.BytewiseComparator"; }
    int Compare(const Slice& a, const Slice& b) const override {return a.compare(b); }

    void FindShortestSeparator(std::string* start, const Slice& limit) const override {
        size_t min_length = std::min(start->size(), limit.size());
        size_t diff_index = 0;
        while(diff_index < min_length && (*start)[diff_index] == limit[diff_index]) 
            diff_index++;
        if (diff_index < min_length) {
            uint8_t diff_byte = static_cast<uint8_t>((*start)[diff_index]);
            if (diff_byte < static_cast<uint8_t>(limit[diff_index]) 
                && diff_index + 1 < static_cast<uint8_t>(limit[diff_index])) {
                    (*start)[diff_index]++;
                    start->resize(diff_index + 1);
                    assert(Compare(*start, limit)<0);
            }
        }
    }

    void FindShortSuccessor(std::string* key) const override {
        size_t n = key->size();
        for (size_t i = 0; i < n; i++) {
            const uint8_t byte = (*key)[i];
            if (byte != static_cast<uint8_t>(0xff)) {
                (*key)[i] = byte + 1;
                key->resize(i + 1);
                return;
            }
        }
    }   
};
}   // namespace

const Comparator* BytewiseComparator() {
    static BytewiseComparatorImpl instance;
    return &instance;
}

}; // namespace leveldb