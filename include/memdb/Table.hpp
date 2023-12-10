#pragma once

#include <cstdint>
#include <list>
#include <string>
#include <utility>
#include "Record.hpp"

template <class T>
class Table {
  public:
    Table() = default;

    Table(std::uint32_t id, const std::string& name) : _id{id}, _name{name} {}

    template <class... Args>
    Record<T>& insert(std::uint64_t txn, Args&&... args) {
        return _records.emplace_back(txn, std::forward<Args>(args)...);
    }

    auto begin() {
        return _records.begin();
    }

    auto end() {
        return _records.end();
    }

    void purge(std::uint64_t currentTxnId) {
        for (auto iter = _records.begin(); iter != _records.end();) {
            Record<T>& record = *iter;

            if (record.canBePurged(currentTxnId)) {
                iter = _records.erase(iter);
            } else {
                ++iter;
            }
        }
    }

  private:
    using RecordPtr = Record<T>*;
    std::uint32_t _id;
    std::string _name;
    std::list<Record<T>> _records;
};