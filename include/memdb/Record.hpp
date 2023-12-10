#pragma once

#include "Types.hpp"
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <utility>

template <class T>
class Record {
  public:
    template <class... Args>
    Record(const TxnId newTxnId, Args&&... args) : _txnIdAdded{newTxnId}, data{std::forward<Args>(args)...} {}

    explicit operator const T&() const {
        return data;
    }

    const T& operator*() const {
        return data;
    }

    void markDeleted(const TxnId newTxnId) {
        if (!deleted()) {
            _txnIdDeleted = newTxnId;
        }
    }

    bool deleted() const {
        return static_cast<bool>(_txnIdDeleted);
    }

    bool canBeRead(const TxnId txnId) const {
        return (!deleted() || _txnIdDeleted > txnId) && _txnIdAdded <= txnId;
    }

    bool canBePurged(const TxnId txnId) const {
        return deleted() && _txnIdDeleted <= txnId;
    }

  private:
    TxnId _txnIdDeleted{};
    TxnId _txnIdAdded{};
    T data;
};
