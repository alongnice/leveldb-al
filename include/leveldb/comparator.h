/**
 * @file comparator.h
 * @author alongnice
 * @brief 比较器基类,完成了一些场景的业务优化
 * 1. SSTable排序: 确保磁盘中键的顺序性
 * 2. Memtable阻止: 内存中数据的排序结构
 * 3. 查询优化: 支持高效的范围查询
 * 4. 合并操作: 多个有序内容的合并
 * 
 * 扩展可能性:
 * 1. 数值比较器(数值比较,字典序比较)
 * 2. 时间戳比较
 * 3. 复合键比较(多字段组合键排序)
 * 4. 反向比较器(逆序排列)
 * @version 0.1
 * @date 2025-07-15
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once
#include <string>

namespace leveldb {
class Slice;

class Comparator {
public:
    virtual ~Comparator();

    // 传统比较逻辑 > == <
    virtual int Compare(const Slice& a, const Slice& b) const = 0;

    // 比较器的名称。用于检查比较器不匹配(即，使用一个比较器创建的DB被另一个比较器访问。
    // 每当比较器的实现发生变化，导致任意两个键的相对顺序发生变化时，这个包的客户端应该切换到一个新的名称。
    // 以“leveldb”开头的名称是保留的，不应由该软件包的任何客户端使用。
    virtual const char* Name() const = 0;

    // 高级函数:这些函数用于减少内部数据结构(如索引块)的空间需求。
    // 如果*start < limit，则将*start更改为[start，limit]中的短字符串。
    // 简单的比较器实现可能返回不变的*start，也就是说，不做任何事情的该方法的实现是正确的。
    virtual void FindShortestSeparator(std::string* start, const Slice& limit) const = 0;


    // 将*键更改为短字符串> = *键。
    // 简单的比较器实现可能返回不变的*key，即，不做任何事情的该方法的实现是正确的。
    virtual void FindShortSuccessor(std::string* key) const = 0;
};

// 返回一个内置的比较器，该比较器使用按字节排序的词典。结果仍然是该模块的属性，不得删除。
const Comparator* BytewiseComparator();
}

// 全纯虚函数允许不同的比较策略
// 数据库系统中,键的排序策略影响:
// 1. 键的存储顺序
// 2. 键的查找效率
// 3. 键的范围查询性能

// 支持 字节序比较 数值比较 自定也业务逻辑比较 国际化排序

// Name 范围比较器名称标识
// 用途: 兼容性, 版本控制, 错误预防

// findShortestSeparator 两个键之间找到最短分隔符
// 用途: 减少存储空间, 提高查询效率,加快键的比较速度
// apple banana 那么apple < b < banana

// findShortSuccessor 找到键的短后继
// 用途: 查询范围边界优化, 索引结构节省, 提升查询性能
// abc b作为后继 abc < b


// 深层思考 findShortestSeparator 和 findShortSuccessor 的设计意图
// 存储优化降低磁盘空间 内存优化减少索引大小 查询优化更短的键比较

