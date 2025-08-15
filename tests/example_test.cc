#include <gtest/gtest.h>
#include "slice.h"
#include "status.h"

// 集成测试或示例测试
TEST(IntegrationTest, BasicUsage) {
    // 测试Slice和Status的基本配合使用
    leveldb::Slice key("test_key");
    leveldb::Status status = leveldb::Status::OK();
    
    EXPECT_TRUE(status.ok());
    EXPECT_EQ(key.size(), 8);
}
