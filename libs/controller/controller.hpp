
#include "../model/model.hpp"
#include "../view/view.hpp"
#include <string>
#include <memory>

class EditorController {
public:
    EditorController();
    
    void newFile(const std::string& filename);
    void importFile(const std::string& filename);
    void exportFile(const std::string& filename);
    void addShape(std::shared_ptr<Shape> shape);
    void removeShape(size_t index);
    void updateFile(const std::string& fileName);

private:
    std::shared_ptr<Model> model;
    std::shared_ptr<EditorView> view;
    
    void updateView();
};
