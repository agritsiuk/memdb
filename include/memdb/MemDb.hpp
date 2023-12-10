#pragma once

#include <cstdint>
#include <list>
#include <string>
#include <unordered_map>
#include "Record.hpp"
#include "Table.hpp"

template <class T>
class MemDb {
  public:
    using TablePtr = Table<T>*;

    Table<T>& table(std::uint32_t tableId, const std::string& tableName) {
        TablePtr table = tableById(tableId);
        if (table) {
            return *table;
        }

        auto& result = _tables.emplace_back(tableId, tableName);
        _tablesById.emplace(tableId, &result);
        _tablesByName.emplace(tableName, &result);

        return result;
    }

    std::uint64_t currentTxnId() const {
        return _currentTxnId;
    }

    std::uint64_t newTxnId() {
        return _currentTxnId + 1;
    }

    void commit() {
        _currentTxnId = newTxnId();
    }

    TablePtr tableById(std::uint32_t tableId) {
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

    void purge(std::uint64_t minTxnId) {
        for (auto& table : _tables) {
            table.purge(minTxnId);
        }
    }

  private:
    std::list<Table<T>> _tables;
    std::uint64_t _currentTxnId{};
    std::unordered_map<std::uint32_t, TablePtr> _tablesById;
    std::unordered_map<std::string, TablePtr> _tablesByName;
};