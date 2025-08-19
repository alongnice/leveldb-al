/**
 * @file random.h
 * @author alongnice
 * @brief 
 * @version 0.1
 * @date 2025-08-19
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <cstdint>
namespace leveldb {
class Random{

private:
    uint32_t seed_;
public:
    explicit Random(uint32_t s): seed_(s & 0x7fffffffu){
        if(seed_ == 0 || seed_ == 2147483647L) seed_ = 1;
    }

    uint32_t Next(){
        static const uint32_t M = 2147483647L; // 2^31 - 1
        static const uint64_t A = 16807;        // bits 14 8 7 5 2 1 0(斐波那契)

        uint64_t product= seed_* A;
        seed_ = static_cast<uint32_t>((product>>31)+(product & M));
        if(seed_ > M) seed_ -= M;
        return seed_;
    }

    uint32_t Uniform(int n){return Next() % n;}

    bool OneIn(int n){return (Next() % n) == 0;}

    uint32_t Skewed(int max_log){
        return Uniform(1 << Uniform(max_log + 1));
    }
};  // class Random
}   // namespace leveldb

/**
 * seed_：随机数生成器的种子，初始化时确保非零
 * 保证种子要在 1-2^31-2 的范围之内 也就是非负int类型的数目范围(不包含边界)
 * 
 * Next 是一个经典的线性同余随机数算法(Lehmer RNG) 参数选用Minimal Standard 
 * 2^31-1 是模数 16807是个经典的质数
 * 能保证在范围内循环 周期长
 * >>31 &M 是2中快速取模的技巧避免慢速除法
 * 
 * Uniform 负责产生均匀分布的随机数
 * 
 * Oneln 负责产生指定概率的true
 * 
 * Skewed 在0-max_log之间选择一个基数
 * 然后再 0-2^base-1 之间选择一个数
 * 取更小的结果 模拟某些分布例如调表层数
 * 
 * 已知这个随机数生成去针对密码学不足够安全
 * 但是针对跳表层数 是没有问题的经典高效
 */