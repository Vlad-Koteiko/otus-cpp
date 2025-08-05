#include "lib.hpp"
#include <cassert>
#include <cstdint>
#include <iostream>
#include <map>

int main(int, char **) {
  std::cout << "Version: " << version() << std::endl;

  try {
    {
      std::map<int, std::uint64_t, std::less<int>,
               PoolAllocator<std::pair<const int, std::uint64_t>, 10>>
          foo;

      for (int i = 0; i < 10; ++i) {
        foo.insert({i, factorial(i)});
      }

      // foo.insert({11,11}); // bad alloc

      for (auto [f, s] : foo) {
        std::cout << " first " << f << " second " << s << std::endl;
      }
    }
    //--------------------------------------------------------------------
    {
      CustomContainer<int> container;

      std::cout << "Container size: " << container.size() << " empty "
                << container.empty() << std::endl;
      for (int i = 0; i < 10; ++i) {
        container.push_back(i);
      }

      std::cout << "Container elements: ";
      for (auto it = container.begin(); it != container.end(); ++it) {
        std::cout << *it << " ";
      }

      std::cout << std::endl;

      std::cout << "Container size: " << container.size() << " empty "
                << container.empty() << std::endl;


      CustomContainer<int, PoolAllocator<int, 10>> containerWithAlloc;
      containerWithAlloc.push_back(42);
      containerWithAlloc.push_back(43);
      containerWithAlloc.push_back(44);
      containerWithAlloc.push_back(45);
      containerWithAlloc.push_back(46);
      containerWithAlloc.push_back(47);
      containerWithAlloc.push_back(48);
      containerWithAlloc.push_back(49);
      containerWithAlloc.push_back(50);
      containerWithAlloc.push_back(51);
      // containerWithAlloc.push_back(52); // bad alloc

      std::cout << "Container with custom allocator: ";
      for (auto val : containerWithAlloc) {
        std::cout << val << " ";
      }
    }
    std::cout << std::endl;

  } catch (...) {

    std::cerr << " Exception !" << std::endl;
  }

  return 0;
}
