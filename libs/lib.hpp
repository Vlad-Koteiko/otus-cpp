#pragma once
#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <memory>

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
      : pool(), ptr(reinterpret_cast<std::uint8_t *>(pool.data())),
        ptrMax(reinterpret_cast<std::uint8_t *>(pool.data() + N)) {}

  template <typename U>
  PoolAllocator(const PoolAllocator<U, N> &)
      : pool(), ptr(reinterpret_cast<std::uint8_t *>(pool.data())),
        ptrMax(reinterpret_cast<std::uint8_t *>(pool.data() + N)) {}

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

//---------------------------------------------------------------------------------

template <typename T, typename Allocator = std::allocator<T>>
class CustomContainer {
private:
  struct ListNode {
    T data;
    ListNode *next;

    ListNode() : data(), next(nullptr) {}
    ListNode(const T &value) : data(value), next(nullptr) {}
  };

  using NodeAllocator = typename std::allocator_traits<
      Allocator>::template rebind_alloc<ListNode>;
  using NodeAllocatorTraits = std::allocator_traits<NodeAllocator>;

  ListNode *head;
  ListNode *tail;
  NodeAllocator allocator;
  size_t size_;

public:
  class Iterator {
  private:
    ListNode *current;

  public:
    Iterator(ListNode *node) : current(node) {}

    T &operator*() const { return current->data; }
    T *operator->() const { return &current->data; }

    Iterator &operator++() {
      current = current->next;
      return *this;
    }

    Iterator operator++(int) {
      Iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    bool operator==(const Iterator &other) const {
      return current == other.current;
    }
    bool operator!=(const Iterator &other) const { return !(*this == other); }
  };

  Iterator begin() { return Iterator(head); }
  Iterator end() { return Iterator(nullptr); }

  CustomContainer(const Allocator &alloc = Allocator())
      : head(nullptr), tail(nullptr), allocator(alloc),size_(0) {}

  ~CustomContainer() { clear(); }

  void push_back(const T &value) {
    ListNode *newNode = NodeAllocatorTraits::allocate(allocator, 1);
    NodeAllocatorTraits::construct(allocator, newNode, value);

    if (!head) {
      head = tail = newNode;
    } else {
      tail->next = newNode;
      tail = newNode;
    }
    ++size_;
  }

  void clear() {
    while (head) {
      ListNode *next = head->next;
      NodeAllocatorTraits::destroy(allocator, head);
      NodeAllocatorTraits::deallocate(allocator, head, 1);
      head = next;
    }
    tail = nullptr;
    size_ = 0;
  }

  [[nodiscard]] bool empty() const { return head == nullptr; }
  [[nodiscard]] std::size_t size () const { return size_; }
};
