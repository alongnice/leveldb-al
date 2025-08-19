/**
 * @file arena.h
 * @author alongnice
 * @brief 内存池, 用于管理内存分配和释放
 *  几种的申请释放要对比系统调用陷入内核态的效率更高
 * @version 0.1
 * @date 2025-08-19
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once
#include <atomic>
#include <cassert>
#include <cstdint>
#include <cstddef>
#include <vector>


namespace leveldb {
class Arena {
public:
    /**
     * @brief 默认构造函数
     */
    Arena();
    Arena(const Arena&) = delete; // 禁止拷贝构造
    /**
     * @brief 析构函数
     * 
     */
    ~Arena();

    /**
     * @brief 申请内存快
     * @param bytes 申请内存块大小
     * @return char* 返回申请的内存块指针(? 大小在外部保存了?)
     */
    char* Allocate(size_t bytes);

    /**
     * @brief 强制使用malloc进行内存分配 保证返回的内存块地址是8字节对齐的
     *  应该是存在一些问题无法断定malloc 分配的内存宽度一定是8个字节
     * @param bytes 申请内存块大小
     * @return char* 返回申请的内存块指针
     */
    char* AllocateAligned(size_t bytes);

    /**
     * @brief 返回内存块大小
     * @return size_t 
     */
    size_t MemoryUsage() const {
        return memory_usage_.load(std::memory_order_relaxed);
    }

private:
    /**
     * @brief 分配回退
     * 
     * @param bytes 
     * @return char* 
     */
    char* AllocateFallback(size_t bytes);
    /**
     * @brief 分配新块
     * 
     * @param block_bytes 
     * @return char* 
     */
    char* AllocateNewBlock(size_t block_bytes);

    /**
     * @brief 分配状态
     */
    char* alloc_ptr_;
    size_t alloc_bytes_remaining_;

    /**
     * @brief 内存块数组
     * 
     */
    std::vector<char*> blocks_;


    /**
     * @brief 内存块大小
     * 
     */
    std::atomic<size_t> memory_usage_;

};  // class Arena

inline char* Arena::Allocate(size_t bytes){
    // 判断参数是否合规
    assert(bytes > 0);
    // 尝试从当前分配块中分配内存
    if(bytes <= alloc_bytes_remaining_) {
        char* result = alloc_ptr_;
        alloc_ptr_ += bytes;
        alloc_bytes_remaining_ -= bytes;
        memory_usage_.fetch_add(bytes, std::memory_order_relaxed);
        return result;
    } else {
        // 分配新的内存块
        return AllocateFallback(bytes);
    }
}

}  // namespace leveldb

/**
 * 核心成员变量
 * alloc_ptr_：当前可分配内存块的指针
 * alloc_bytes_remaining_：当前块剩余可分配字节数
 * blocks_：已分配内存块的指针数组
 * memory_usage_：总内存使用量
 * 
 * 
 * 基本流程
 * 小内存申请（如 < kBlockSize/4）
 * 优先从当前块分配（Allocate）
 * 当前块空间不足时，AllocateFallback 分配新块（kBlockSize），并更新 alloc_ptr_ 和 alloc_bytes_remaining_
 * 大内存申请（如 > kBlockSize/4）
 * 直接分配一个独立的新块（AllocateNewBlock），避免浪费主块空间
 * 
 * 
 * 增删改查说明
 * 增（Allocate/AllocateAligned）：分配新内存，更新指针和剩余空间
 * 删（~Arena）：析构时统一释放所有 blocks_ 指向的内存
 * 改：无直接“改”操作，分配后由外部使用者管理
 * 查（MemoryUsage）：查询当前内存池总分配量
 * 
 * 
 * 对齐分配（AllocateAligned）
 * 预留接口，后续可实现按 8 字节或更高对齐分配，保证特殊场景下的性能和正确性
 * 总结：
 * Arena  内存池通过预分配大块内存，减少系统调用，提升分配效率。小内存请求复用主块空间，大内存请求单独分配，析构时统一释放，适合高性能场景。
 *
 */

/**
 * 对比之前仿照 nginx 的内存池实现对比
 * https://gist.github.com/alongnice/f50c4e8bce82a543870c995e87f65eb3
 * 
 * 区别在于 nginx 的内存池有更加复杂的引用计数 失败计数 链表管理 和监控接口 适合高并发场景
 * Arena 则更加简洁,因为目前本身不需要复杂的内存管理机制
 *
 */