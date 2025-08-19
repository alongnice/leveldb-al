/**
 * @file arena.cc
 * @author alongnice
 * @brief 
 * @version 0.1
 * @date 2025-08-19
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "../../include/leveldb/arena.h"

namespace leveldb {

static const int kBlockSize = 4096; // 每个内存块的大小

Arena::Arena() 
    : alloc_ptr_(nullptr), alloc_bytes_remaining_(0), memory_usage_(0) {}
    // 初始化内存池

Arena::~Arena(){
    // 释放所有分配的内存块
    for(size_t it = 0; it < blocks_.size(); ++it) {
        delete[] blocks_[it];
    }
}

char* Arena::AllocateFallback(size_t bytes) {
    if (bytes>kBlockSize/4){
        char* result = AllocateNewBlock(bytes);
        return result;
    }

    alloc_ptr_ = AllocateNewBlock(kBlockSize);
    alloc_bytes_remaining_ = kBlockSize;

    char* result = alloc_ptr_;
    alloc_ptr_ += bytes;
    alloc_bytes_remaining_ -= bytes;
    return result;
}

// char* Arena::AllocateAligned(size_t bytes){
//     const int align = (sizeof(void*) > 8) ? sizeof(void*) : 8;
//     static_assert((align & (align - 1)) == 0, "Alignment must be a power of 2");

//     size_t current_mod = reinterpret_cast<uintptr_t>(alloc_ptr_) & (align-1);
//     size_t slop = (current_mod == 0) ? 0 : align - current_mod;
//     size_t needed = bytes + slop;

//     char* result;
//     if(needed <= alloc_bytes_remaining_) {
//         result = alloc_ptr_ + slop;
//         alloc_ptr_ += needed;
//         alloc_bytes_remaining_ -= needed;
//     }else result = AllocateFallback(bytes);
//     assert((reinterpret_cast<uintptr_t>(result) & (align - 1)) == 0);
//     return result;
// }

char* Arena::AllocateAligned(size_t bytes) {
    const int align = (sizeof(void*) > 8) ? sizeof(void*) : 8;
    static_assert((align & (align - 1)) == 0, "Pointer size should be a power of 2");
    size_t current_mod = reinterpret_cast<uintptr_t>(alloc_ptr_) & (align - 1);
    size_t slop = (current_mod == 0 ? 0 : align - current_mod);
    size_t needed = bytes + slop;
    char* result;
    if (needed <= alloc_bytes_remaining_) {
        result = alloc_ptr_ + slop;
        alloc_ptr_ += needed;
        alloc_bytes_remaining_ -= needed;
    } else {
        // AllocateFallback always returned aligned memory
        result = AllocateFallback(bytes);
    }
    assert((reinterpret_cast<uintptr_t>(result) & (align - 1)) == 0);
    return result;
}

// char* Arena::AllocateNewBlock(size_t block_bytes) {
//     char* result = new char[block_bytes];
//     blocks_.push_back(result);
//     memory_usage_.fetch_add(block_bytes + sizeof(char*),std::memory_order_relaxed);
//     // 不仅增加了分配快的大小 还增加了 char*的大小 这是管理开销
//     // 但是测试时并未录入 没有考虑这个部分

//     // 距离申请20字节 测试项增加20
//     // 但是分配则是4096+sizeof(char*)

//     return result;
// }

char* Arena::AllocateNewBlock(size_t block_bytes) {
    char* result = new char[block_bytes];
    blocks_.push_back(result);
    memory_usage_.fetch_add(block_bytes + sizeof(char*), std::memory_order_relaxed);
    return result;
}

}   // namespace leveldb
