#include "../../include/leveldb/status.h"
#include <cstdio>
#include <cstdint>

namespace leveldb {

/**
 * @brief 拷贝状态信息
 * 
 * @param state 
 * @return const char* 
 */
const char* Status::CopyState(const char* state) {
    // 断言检测, 大小提取(前四个字节是大小),
    assert(state != nullptr);
    const uint32_t size = *reinterpret_cast<const uint32_t*>(state);
    // 分配内存, 拷贝数据
    char* result = new char[size + 5];
    std::memcpy(result, state, size + 5);
    return result;
}

/**
 * @brief 完整状态信息构建
 * 
 * @param code 
 * @param msg 主消息
 * @param msg2 附加消息
 */
Status::Status(Code code, const Slice& msg, const Slice& msg2) {
    // 判空,取大小
    assert(code != kOk);
    const uint32_t len1 = static_cast<uint32_t>(msg.size());
    const uint32_t len2 = static_cast<uint32_t>(msg2.size());
    // 主消息和附加消息中间由 ": "
    const uint32_t size = len1 + (len2 ? (2 + len2) : 0);
    // 申请空间,然后逐步导入
    char* result = new char[size + 5];
    std::memcpy(result, &size, sizeof(size));
    result[4] = static_cast<char>(code);
    std::memcpy(result + 5, msg.data(), len1);
    // 存在附加消息的花
    if (len2) {
        result[5 + len1] = ':';
        result[6 + len1] = ' ';
        std::memcpy(result + 7 + len1, msg2.data(), len2);
    }
    state_ = result;
}

/**
 * @brief 将状态转换为字符串
 * 
 * @return std::string 
 */
std::string Status::ToString() const {
    // 空则状态正常
    if (state_ == nullptr) {
        return "OK";
    } else {
        // 临时缓冲空间
        char tmp[30];
        const char* type;
        switch (code()) {
            case kNotFound: type = "NotFound"; break;
            case kCorruption: type = "Corruption"; break;
            case kNotSupported: type = "NotSupported"; break;
            case kInvalidArgument: type = "InvalidArgument"; break;
            case kIOError: type = "IOError"; break;
            default: 
                std::snprintf(tmp, sizeof(tmp),
                            "Unknown code %d", static_cast<int>(code()));
                type = tmp;
                break;
        }
        // 拼接状态信息
        std::string result(type);
        result.append(": ");  // 添加这行！之前可能漏了
        uint32_t length;
        std::memcpy(&length, state_, sizeof(length));
        result.append(state_ + 5, length);
        return result;
    }
}

} // namespace leveldb