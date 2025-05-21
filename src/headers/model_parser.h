#ifndef S21_MODEL_PARSER_H_
#define S21_MODEL_PARSER_H_

#include <string>
#include <vector>

namespace s21 {

struct ObjData {
    std::vector<double> vertices;
    std::vector<int> edges;
    int vertex_count;
    int edge_count;

    ObjData() : vertex_count(0), edge_count(0) {}
};

class ModelObserver;

class ParserModel {
private:
    ObjData data_;
    std::vector<ModelObserver*> observers_;
    double scale_;
    double move_x_, move_y_, move_z_;
    double rotate_x_, rotate_y_, rotate_z_;

public:
    ParserModel();
    ~ParserModel() = default;

    void loadFromFile(const std::string& file_path);
    void addObserver(ModelObserver* observer);
    void removeObserver(ModelObserver* observer);
    void notifyObservers();
    const ObjData& getData() const;

    void moveX(double value);
    void moveY(double value);
    void moveZ(double value);
    void rotateX(double angle);
    void rotateY(double angle);
    void rotateZ(double angle);
    void scale(double ratio);

    double getRotateX() const { return rotate_x_; }
    double getRotateY() const { return rotate_y_; }
    double getRotateZ() const { return rotate_z_; }
};

}  // namespace s21

#endif  // S21_MODEL_PARSER_H_
