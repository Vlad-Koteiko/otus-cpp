#include "controller.hpp"

EditorController::EditorController()
    : model(std::make_shared<Model>()), view(std::make_shared<EditorView>()) {}

void EditorController::newFile(const std::string &filename) {
  [[likely]] if (model->newFile(filename) && view) {
    updateFile(filename);
  } else if (view) {
    view->showMessage("Failed to new document");
  }
}

void EditorController::importFile(const std::string &filename) {
  [[likely]] if (model->importFromFile(filename)) {
    updateFile(filename);
  } else if (view) {
    view->showMessage("Failed to import document");
  }
}

void EditorController::exportFile(const std::string &filename) {
  [[likely]] if (model->exportToFile(filename) && view) {
    updateFile(filename);
  } else if (view) {
    view->showMessage("Failed to export document");
  }
}

void EditorController::addShape(std::shared_ptr<Shape> shape) {
  model->addShape(shape);
  updateView();
}

void EditorController::removeShape(size_t index) {
  model->removeShape(index);
  updateView();
}

void EditorController::updateView() {
  if (view) {
    view->draw(model->getShapes());
  }
}

void EditorController::updateFile(const std::string &fileName) {
  if (view) {
    view->file(fileName);
  }
}
