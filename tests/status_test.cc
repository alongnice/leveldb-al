#include <gtest/gtest.h>
#include "leveldb/status.h"

TEST(StatusTest, OK) {
    leveldb::Status s = leveldb::Status::OK();
    EXPECT_TRUE(s.ok());
    EXPECT_FALSE(s.IsNotFound());
    EXPECT_FALSE(s.IsCorruption());
    EXPECT_FALSE(s.IsNotSupported());
    EXPECT_FALSE(s.IsInvalidArgument());
    EXPECT_FALSE(s.IsIOError());
    EXPECT_EQ(s.ToString(), "OK");
}

TEST(StatusTest, NotFound) {
    leveldb::Status s = leveldb::Status::NotFound("key not found");
    EXPECT_FALSE(s.ok());
    EXPECT_TRUE(s.IsNotFound());
    EXPECT_FALSE(s.IsCorruption());
    EXPECT_EQ(s.ToString(), "NotFound: key not found");
}

TEST(StatusTest, NotFoundWithTwoMessages) {
    leveldb::Status s = leveldb::Status::NotFound("key not found", "in table users");
    EXPECT_FALSE(s.ok());
    EXPECT_TRUE(s.IsNotFound());
    EXPECT_EQ(s.ToString(), "NotFound: key not found: in table users");
}

TEST(StatusTest, Corruption) {
    leveldb::Status s = leveldb::Status::Corruption("bad checksum");
    EXPECT_FALSE(s.ok());
    EXPECT_TRUE(s.IsCorruption());
    EXPECT_FALSE(s.IsNotFound());
    EXPECT_EQ(s.ToString(), "Corruption: bad checksum");
}

TEST(StatusTest, IOError) {
    leveldb::Status s = leveldb::Status::IOError("disk full", "/tmp/data");
    EXPECT_FALSE(s.ok());
    EXPECT_TRUE(s.IsIOError());
    EXPECT_EQ(s.ToString(), "IOError: disk full: /tmp/data");
}

TEST(StatusTest, InvalidArgument) {
    leveldb::Status s = leveldb::Status::InvalidArgument("invalid parameter");
    EXPECT_FALSE(s.ok());
    EXPECT_TRUE(s.IsInvalidArgument());
    EXPECT_EQ(s.ToString(), "InvalidArgument: invalid parameter");
}

TEST(StatusTest, NotSupported) {
    leveldb::Status s = leveldb::Status::NotSupported("operation not supported");
    EXPECT_FALSE(s.ok());
    EXPECT_TRUE(s.IsNotSupported());
    EXPECT_EQ(s.ToString(), "NotSupported: operation not supported");
}

TEST(StatusTest, CopyConstructor) {
    leveldb::Status s1 = leveldb::Status::NotFound("original error");
    leveldb::Status s2(s1);
    
    EXPECT_TRUE(s2.IsNotFound());
    EXPECT_EQ(s1.ToString(), s2.ToString());
}

TEST(StatusTest, CopyAssignment) {
    leveldb::Status s1 = leveldb::Status::IOError("io error");
    leveldb::Status s2 = leveldb::Status::OK();
    
    s2 = s1;
    EXPECT_TRUE(s2.IsIOError());
    EXPECT_EQ(s1.ToString(), s2.ToString());
}

TEST(StatusTest, MoveConstructor) {
    leveldb::Status s1 = leveldb::Status::Corruption("corruption error");
    std::string original_str = s1.ToString();
    
    leveldb::Status s2(std::move(s1));
    EXPECT_TRUE(s2.IsCorruption());
    EXPECT_EQ(s2.ToString(), original_str);
}

TEST(StatusTest, MoveAssignment) {
    leveldb::Status s1 = leveldb::Status::InvalidArgument("invalid arg");
    leveldb::Status s2 = leveldb::Status::OK();
    std::string original_str = s1.ToString();
    
    s2 = std::move(s1);
    EXPECT_TRUE(s2.IsInvalidArgument());
    EXPECT_EQ(s2.ToString(), original_str);
}
