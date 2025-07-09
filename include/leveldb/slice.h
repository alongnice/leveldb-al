/**
 * @file slice.h
 * @author alongnice
 * @brief slice 可以理解为一个字符串的视图，它并不拥有数据，而是指向某个数据的起始位置和长度。
 *        这样可以避免不必要的内存拷贝，提高效率。
 *        Slice 类提供了对字符串的只读访问，常用于 LevelDB 中的键值对操作。
 *        雷同设计还有: std::string_view, gRPC的byteBuffer, go的不可变字符串
 * @version 0.1
 * @date 2025-07-04
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once
#include <cassert>
#include <cstddef>
#include <cstring>
#include <string>

namespace leveldb {

class Slice {
public:
	
    Slice() : data_(""), size_(0) {}
    // 构造函数(空)

	Slice(const char* d, size_t n): data_(d), size_(n) {}
    // 构造函数(指针和长度)

	Slice(const std::string& s): data_(s.data()), size_(s.size()) {}
    // 构造函数(字符串对象)

	Slice(const char* s): data_(s), size_(s ? strlen(s) : 0) {}
    // 构造函数(字符串字面量|裸指针)

    // Slice(const Slice& other) : data_(other.data_), size_(other.size_) {}
    // 拷贝构造函数(默认)手动实现方案
    Slice(const Slice&) = default;
    // 拷贝构造函数(默认)自动构造方案,跟前一个生成机器码一样但是不用手动管理心智负担少
    Slice& operator=(const Slice&) = default;
    // 拷贝赋值运算符(默认)

    const char* data() const {return data_; }
    // 获取数据指针

	size_t size() const {return size_; }
    // 获取数据长度

	bool empty() const {return size_ == 0; }
    // 判断数据是否为空

    const char* begin() const { return data(); }
    const char* end() const { return data()+size(); }
    // 获取数据的起始和结束指针

	char operator[](size_t n) const{
        assert(n< size()); //release 模式下不检查
        return data_[n];
    }
    // 下标运算符重载

    void clear() {data_ = ""; size_ = 0; }
    // 清空数据

	void remove_prefix(size_t n){
        assert(n <= size()); //release 模式下不检查
        data_ += n;
        size_ -= n;
    }
    // 移除前缀数据

	std::string ToString() const {return std::string(data_, size_); }
    // 将 Slice 转换为 std::string

	int compare(const Slice& b) const;


	bool starts_with(const Slice& b) const{
        return((size_>= b.size_) && (memcmp(data_, b.data_, b.size_) == 0));
    }
private:
	const char* data_;  // 指向数据的指针
	size_t size_;       // 数据的长度
};

// 对比运算符重载
inline bool operator==(const Slice& a, const Slice& b) {
    return (a.size() == b.size() && memcmp(a.data(), b.data(), a.size()) == 0);
}

// 不等于运算符重载
inline bool operator!=(const Slice& a, const Slice& b) {
    return !(a == b);
}

inline int Slice::compare(const Slice& b) const {
    const size_t min_size = (size_ < b.size_) ? size_ : b.size_;
    int r = memcmp(data_, b.data_, min_size);
    if(r == 0 && size_ < b.size_) r=-1;
    if(r == 0 && size_ > b.size_) r=1;
    return r;
}

}  // namespace leveldb
