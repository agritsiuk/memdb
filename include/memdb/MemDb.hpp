#pragma once

#include "Record.hpp"
#include "Table.hpp"
#include "Types.hpp"
#include <cstdint>
#include <list>
#include <string>
#include <unordered_map>

template <class T>
class MemDb {
  public:
    using TablePtr = Table<T>*;

    Table<T>& table(const TableId tableId, const std::string& tableName) {
        TablePtr table = tableById(tableId);
        if (table) {
            return *table;
        }

        auto& result = _tables.emplace_back(tableId, tableName);
        _tablesById.emplace(tableId, &result);
        _tablesByName.emplace(tableName, &result);

        return result;
    }

    TxnId lastCommittedTxnId() const {
        return _lastCommittedTxnId;
    }

    TxnId newTxnId() const {
        return _newTxnId;
    }

    void commit() {
        ++_lastCommittedTxnId;
        ++_newTxnId;
    }

    TablePtr tableById(const TableId tableId) {
        if (const auto iter = _tablesById.find(tableId); iter != _tablesById.end()) {
            return iter->second;
        }

        return nullptr;
    }

    TablePtr tableByName(const std::string& tableName) {
        if (const auto iter = _tablesByName.find(tableName); iter != _tablesByName.end()) {
            return iter->second;
        }

        return nullptr;
    }

    auto begin() {
        return _tables.begin();
    }

    auto end() {
        return _tables.end();
    }

    void purge() {
        for (auto& table : _tables) {
            table.purge(_lastCommittedTxnId);
        }
    }

  private:
    std::list<Table<T>> _tables;
    TxnId _lastCommittedTxnId{0};
    TxnId _newTxnId{1};
    std::unordered_map<TableId, TablePtr> _tablesById;
    std::unordered_map<std::string, TablePtr> _tablesByName;
};