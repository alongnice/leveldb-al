#include "leveldb/slice.h"

namespace leveldb {
Slice::Slice(const char* d, size_t n) : data_(d), size_(n) {}
}  // namespace leveldb
