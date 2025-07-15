/**
 * @file status.h
 * @author alongnice
 * @brief 状态管理
 * @version 0.1
 * @date 2025-07-09
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once
#include <string>
#include <algorithm>

// #include "leveldb/export.h"
#include "slice.h"

namespace leveldb {

class Status {
public:
    // 构造函数
    Status() noexcept : state_(nullptr) {}
    // noexcept 确保不抛出异常 成员初始化为空

    // 拷贝构造函数 从另一个对象中拷贝过来(深拷贝)
    Status(const Status& rhs);
    // 拷贝赋值运算符 从另一个对象中赋值过来(深拷贝)
    Status& operator=(const Status& rhs);

    // 移动构造函数 从另一个对象中移动过来(浅拷贝) 偷取指针 禁止异常
    Status(Status&& rhs) noexcept : state_(rhs.state_) { rhs.state_ = nullptr; }
    // 移动赋值运算符 从另一个对象中移动过来(浅拷贝) 偷取指针
    Status& operator=(Status&& rhs) noexcept;

    // 析构函数 因为明显的内存布局包含了消息长度+错误代码+消息内容
    ~Status() { delete[] state_; }
    
    static Status OK() { return Status();}


    // 数据未找到,可选2个部分,主内容必要,附加内容如果未传入则为空
    static Status NotFound(const Slice& msg, const Slice& msg2 = Slice()) {
        return Status(kNotFound, msg, msg2);
    }
    static Status Corruption(const Slice& msg, const Slice& msg2 = Slice()) {
        return Status(kCorruption, msg, msg2);
    }
    static Status NotSupported(const Slice& msg, const Slice& msg2 = Slice()) {
        return Status(kNotSupported, msg, msg2);
    }
    static Status InvalidArgument(const Slice& msg, const Slice& msg2 = Slice()) {
        return Status(kInvalidArgument, msg, msg2);
    }
    static Status IOError(const Slice& msg, const Slice& msg2 = Slice()) {
        return Status(kIOError, msg, msg2);
    }


    // 判断状态
    bool ok() const { return state_ == nullptr; }
    bool IsNotFound() const { return code() == kNotFound; }
    bool IsCorruption() const { return code() == kCorruption; }
    bool IsNotSupported() const { return code() == kNotSupported; }
    bool IsInvalidArgument() const { return code() == kInvalidArgument; }
    bool IsIOError() const { return code() == kIOError; }

    // 获取状态码
    std::string ToString() const;

    private:
    enum Code{
        kOk = 0,          // 成功
        kNotFound = 1,    // 未找到
        kCorruption = 2,  // 数据损坏
        kNotSupported = 3,// 不支持的操作
        kInvalidArgument = 4, // 无效的参数
        kIOError = 5,     // IO错误
    };

    // [0..3] 消息长度4字节
    // [4] 错误代码1字节
    // [5..] 错误消息内容
    const char* state_; // 状态信息指针

    Code code() const {
        return (state_ == nullptr) ? kOk : static_cast<Code>(state_[4]);
    }

    Status(Code code, const Slice& msg, const Slice& msg2);
    static const char* CopyState(const char* state);
};

inline Status::Status(const Status& rhs){
    state_ = (rhs.state_ == nullptr) ? nullptr : CopyState(rhs.state_);
}

/**
 * @brief 拷贝赋值运算符
 * 为了避免自己给自己赋值,释放原内存,链式赋值操作
 * @param rhs 
 * @return Status& 
 */
inline Status& Status::operator=(const Status& rhs) {
    if(state_ != rhs.state_) {
        delete[] state_;
        state_ = (rhs.state_ == nullptr) ? nullptr : CopyState(rhs.state_);
    }
    return *this;
}

/**
 * @brief 移动构造函数
 * 避免内存拷贝,直接交付所有权
 * @param rhs 
 * @return Status& 
 */
inline Status& Status::operator=(Status&& rhs) noexcept {
    std::swap(state_, rhs.state_);
    return *this;
}

} // namespace leveldb

/**
 * @brief 状态管理类
 * 值语义类：支持拷贝、移动、赋值
 * 工厂方法模式：通过静态方法创建不同类型的错误
 * RAII模式：自动内存管理
 * 零成本抽象：成功情况无开销
 * 类型安全：编译时保证错误类型正确性
 * 
 */