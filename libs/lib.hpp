#pragma once
#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <new>

int version();

constexpr std::uint64_t factorial(std::uint32_t value) noexcept {

  if (value == 0 || value == 1) {
    return 1;
  }

  return value * factorial(value - 1);
}

template <typename T, size_t N> class PoolAllocator {
private:
  std::array<T, (sizeof(T) * N)> pool;

  std::uint8_t *ptr;
  std::uint8_t *ptrMax;

public:
  using value_type = T;

  PoolAllocator()
      : pool(),
        ptr(reinterpret_cast<std::uint8_t *>(pool.data())),
        ptrMax(reinterpret_cast<std::uint8_t *>(pool.data() + N)) {}

  template <typename U> PoolAllocator(const PoolAllocator<U, N> &) {}

  // THIS IS THE CUSTOM PART - allocate from our pool, not system
  T *allocate(size_t n) {

    size_t bytes_needed = n * sizeof(T);

    if ((ptr + bytes_needed) > ptrMax) {
      std::cout << "Pool exhausted! allocate \n";
      throw std::bad_alloc();
    }

    T *result = reinterpret_cast<T *>(ptr);
    ptr += bytes_needed;
    return result;
  }

  void deallocate([[maybe_unused]] T *ptrData, [[maybe_unused]] size_t n) {

    size_t bytes_needed = n * sizeof(T);
    ptr -= bytes_needed;

    if (ptr < reinterpret_cast<std::uint8_t *>(pool.data())) {

      std::cout << "Pool exhausted! deallocate \n";
      throw std::bad_alloc();
    }
  }

  template <typename U> struct rebind {
    using other = PoolAllocator<U, N>;
  };
};

// Required comparison operators
template <typename T, size_t S1, typename U, size_t S2>
bool operator==(const PoolAllocator<T, S1> &, const PoolAllocator<U, S2> &) {
  return true;
}

template <typename T, size_t S1, typename U, size_t S2>
bool operator!=(const PoolAllocator<T, S1> &, const PoolAllocator<U, S2> &) {
  return false;
}
