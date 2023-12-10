#pragma once

#include "Record.hpp"
#include "Types.hpp"
#include <cstdint>
#include <list>
#include <string>
#include <utility>

template <class T>
class Table {
  public:
    Table() = default;

    Table(const TableId id, const std::string& name) : _id{id}, _name{name} {}

    template <class... Args>
    Record<T>& insert(const TxnId txnId, Args&&... args) {
        return _records.emplace_back(txnId, std::forward<Args>(args)...);
    }

    auto begin() {
        return _records.begin();
    }

    auto end() {
        return _records.end();
    }

    void purge(const TxnId lastCommittedTxnId) {
        for (auto iter = _records.begin(); iter != _records.end();) {
            Record<T>& record = *iter;

            if (record.canBePurged(lastCommittedTxnId)) {
                iter = _records.erase(iter);
            } else {
                ++iter;
            }
        }
    }

  private:
    using RecordPtr = Record<T>*;
    TableId _id;
    std::string _name;
    std::list<Record<T>> _records;
};