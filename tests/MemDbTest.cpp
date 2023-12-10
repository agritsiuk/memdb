#include <string>
#include <gtest/gtest.h>
#include <memdb/MemDb.hpp>
#include <memdb/Record.hpp>

class TestMemDb : public ::testing::Test {
  protected:
    auto& addTestTable() {
        return db.table(1, "testTable");
    }

    MemDb<std::string> db;
};

TEST_F(TestMemDb, Record) {
    Record<std::string> rec{10, "Test"};
    ASSERT_FALSE(rec.deleted());

    ASSERT_FALSE(rec.canBeRead(9));
    ASSERT_TRUE(rec.canBeRead(10));

    ASSERT_FALSE(rec.canBePurged(9));
    ASSERT_FALSE(rec.canBePurged(10));
    rec.markDeleted(11);
    ASSERT_FALSE(rec.canBePurged(9));
    ASSERT_TRUE(rec.canBePurged(11));
    ASSERT_TRUE(rec.canBePurged(12));

    ASSERT_TRUE(rec.canBeRead(10));
    ASSERT_FALSE(rec.canBeRead(11));
}

TEST_F(TestMemDb, TableAdd) {
    ASSERT_EQ(nullptr, db.tableById(1));
    ASSERT_EQ(nullptr, db.tableByName("testTable"));

    auto& table = addTestTable();
    ASSERT_EQ(&table, db.tableById(1));
    ASSERT_EQ(&table, db.tableByName("testTable"));
}

TEST_F(TestMemDb, AddRecord) {
    auto& table = addTestTable();

    ASSERT_EQ(table.begin(), table.end());

    const auto txnId = db.newTxnId();
    auto& rec = table.insert(txnId, "Client1");
    ASSERT_NE(table.begin(), table.end());

    ASSERT_EQ(&rec, &*table.begin());
    ASSERT_EQ(*rec, std::string("Client1"));
}

TEST_F(TestMemDb, TxnId) {
    ASSERT_EQ(0, db.lastCommittedTxnId());
    db.commit();
    ASSERT_EQ(1, db.lastCommittedTxnId());
}

TEST_F(TestMemDb, Purge) {
    auto& table = addTestTable();
    const auto txnId = db.newTxnId();
    auto& rec = table.insert(txnId, "Client1");

    table.purge(db.lastCommittedTxnId());
    ASSERT_NE(table.begin(), table.end());
    db.commit();
    rec.markDeleted(db.lastCommittedTxnId());
    table.purge(db.lastCommittedTxnId());
    ASSERT_EQ(table.begin(), table.end());
}