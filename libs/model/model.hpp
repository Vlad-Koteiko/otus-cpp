#pragma once
#include "../shapes.hpp"
#include <vector>
#include <memory>
#include <string>

class Model {
public:
    Model();
    
    void addShape(std::shared_ptr<Shape> shape);
    void removeShape(size_t index);
    const std::vector<std::shared_ptr<Shape>>& getShapes() const;
    
    bool importFromFile(const std::string& filename);
    bool exportToFile(const std::string& filename) const;
    bool newFile(const std::string& filename);

private:
    std::vector<std::shared_ptr<Shape>> shapes;
};
