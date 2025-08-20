#include "../libs/controller/controller.hpp"

int main(int, char **) {

  auto controller = std::make_shared<EditorController>();
  controller->newFile("newFile.drw");
  controller->importFile("example.drw");
  controller->exportFile("output.drw");
  controller->addShape(std::make_shared<Circle>(Point{10, 20}, 5));
  controller->addShape(std::make_shared<Rectangle>(Point{10, 20}, Point{10, 20}));
  controller->removeShape(0);
  return 0;
}
