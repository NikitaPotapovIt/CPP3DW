#ifndef OBJ_DATA_H
#define OBJ_DATA_H

#include <vector>

namespace s21 {

class ObjData {
public:
    std::vector<double> vertices;
    std::vector<int> edges;
    int vertex_count;
    int edge_count;
};

}  // namespace s21

#endif  // OBJ_DATA_H
