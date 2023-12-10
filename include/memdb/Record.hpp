#pragma once

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <utility>

template <class T>
class Record {
  public:
    template <class... Args>
    Record(std::uint64_t newTxnId, Args&&... args) : _txnIdAdded{newTxnId}, data{std::forward<Args>(args)...} {}

    explicit operator const T&() const {
        return data;
    }

    const T& operator*() const {
        return data;
    }

    void markDeleted(std::uint64_t newTxnId) {
        _txnIdDeleted = newTxnId;
    }

    bool deleted() const {
        return _txnIdDeleted;
    }

    bool canBeRead(std::uint64_t txnId) const {
        return (!_txnIdDeleted || _txnIdDeleted > txnId) && _txnIdAdded <= txnId;
    }

    bool canBePurged(std::uint64_t currentTxnId) const {
        return _txnIdDeleted && _txnIdDeleted <= currentTxnId;
    }

  private:
    std::uint64_t _txnIdDeleted{};
    std::uint64_t _txnIdAdded{};
    T data;
};
