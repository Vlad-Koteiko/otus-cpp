#pragma once
#include <cassert>
#include <iostream>
#include <unordered_map>

int version();

#include <unordered_map>
#include <tuple>
#include <cstddef>
#include <iterator>

template <typename T, T defaultValue>
class Matrix {
private:
    using Pool       = std::unordered_map<std::size_t, std::unordered_map<std::size_t, T>>;
    using InnerMap   = std::unordered_map<std::size_t, T>;
    using RowIter    = typename Pool::iterator;
    using ConstRowIter = typename Pool::const_iterator;
    using ColIter    = typename InnerMap::iterator;
    using ConstColIter = typename InnerMap::const_iterator;
 
    Pool pool_;

    struct ValueProxy {
        Pool &pool;
        std::size_t row;
        std::size_t col;

        operator T() const {
            auto rowIt = pool.find(row);
            if (rowIt != pool.end()) {
                auto colIt = rowIt->second.find(col);
                if (colIt != rowIt->second.end()) {
                    return colIt->second;
                }
            }
            return defaultValue;
        }

        ValueProxy &operator=(T value) {
            if (value == defaultValue) {
                auto rowIt = pool.find(row);
                if (rowIt != pool.end()) {
                    rowIt->second.erase(col);
                    if (rowIt->second.empty()) {
                        pool.erase(rowIt);
                    }
                }
            } else {
                pool[row][col] = value;
            }
            return *this;
        }
    };

    struct RowProxy {
        Pool &pool;
        std::size_t row;

        ValueProxy operator[](std::size_t col) {
            return ValueProxy{pool, row, col};
        }
    };

public:
    [[nodiscard]] RowProxy operator[](std::size_t row) noexcept {
        return RowProxy{pool_, row};
    }

    [[nodiscard]] std::size_t size() const noexcept {
        std::size_t total = 0;
        for (const auto &row : pool_) {
            total += row.second.size();
        }
        return total;
    }

    class Iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type        = std::tuple<std::size_t, std::size_t, T>;
        using difference_type   = std::ptrdiff_t;

        Iterator(RowIter rowIt, RowIter rowEnd)
            : rowIt_(rowIt), rowEnd_(rowEnd)
        {
            if (rowIt_ != rowEnd_) {
                colIt_ = rowIt_->second.begin();
            }
        }

        value_type operator*() const {
            return { rowIt_->first, colIt_->first, colIt_->second };
        }

        Iterator &operator++() {
            ++colIt_;
            while (rowIt_ != rowEnd_ && colIt_ == rowIt_->second.end()) {
                ++rowIt_;
                if (rowIt_ != rowEnd_) {
                    colIt_ = rowIt_->second.begin();
                }
            }
            return *this;
        }

        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const Iterator &other) const {
            return rowIt_ == other.rowIt_ &&
                   (rowIt_ == rowEnd_ || colIt_ == other.colIt_);
        }

        bool operator!=(const Iterator &other) const {
            return !(*this == other);
        }

    private:
        RowIter rowIt_;
        RowIter rowEnd_;
        ColIter colIt_;
    };

    [[nodiscard]] Iterator begin() noexcept {
        return Iterator(pool_.begin(), pool_.end());
    }

    [[nodiscard]] Iterator end() noexcept {
        return Iterator(pool_.end(), pool_.end());
    }
};

