#ifndef LEVELDB_SLICE_H_
#define LEVELDB_SLICE_H_

#include <string>

namespace leveldb {
class Slice {
public:
    Slice(const char* d, size_t n);
private:
    const char* data_;
    size_t size_;
};
}  // namespace leveldb

#endif  // LEVELDB_SLICE_H_
