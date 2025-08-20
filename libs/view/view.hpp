#pragma once
#include "../shapes.hpp"
#include <memory>
#include <vector>

class EditorView {
public:
  virtual void draw(const std::vector<std::shared_ptr<Shape>> &shapes) {
   
    std::cout << "----------------DRAW---------------" << std::endl;
    for (auto &&s : shapes) {
      s->draw();
    }
    std::cout << "-----------------------------------" << std::endl;
  }

  virtual void file(const std::string &filename) {

    std::cout << filename << std::endl;
  }

  virtual void showMessage(const std::string &message) {

    std::cout << message << std::endl;
  }
};
