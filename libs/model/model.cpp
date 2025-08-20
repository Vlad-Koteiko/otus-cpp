#include "model.hpp"
#include <fstream>

Model::Model() {}

void Model::addShape(std::shared_ptr<Shape> shape) {
    shapes.push_back(shape);
}

void Model::removeShape(size_t index) {
    if (index < shapes.size()) {
        shapes.erase(shapes.begin() + index);
    }
}

const std::vector<std::shared_ptr<Shape>>& Model::getShapes() const {
    return shapes;
}

bool Model::newFile(const std::string& filename) {
    std::ifstream file(filename);
    return true; 
}

bool Model::importFromFile(const std::string& filename) {
    std::ifstream file(filename);
    return true; 
}

bool Model::exportToFile(const std::string& filename) const {
    std::ofstream file(filename);
    return true; 
}
