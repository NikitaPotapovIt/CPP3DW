#include "../headers/model_parser.h"
#include "../headers/observer.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cmath>

namespace s21 {

ParserModel::ParserModel()
    : scale_(1.0),
      move_x_(0.0), move_y_(0.0), move_z_(0.0),
      rotate_x_(0.0), rotate_y_(0.0), rotate_z_(0.0) {}

      void ParserModel::loadFromFile(const std::string& file_path) {
        std::ifstream file(file_path);
        if (!file.is_open()) {
            throw std::runtime_error("Unable to open file: " + file_path);
        }
    
        data_ = ObjData();
        std::string line;
        while (std::getline(file, line)) {
            line.erase(0, line.find_first_not_of(" \t"));
            line.erase(line.find_last_not_of(" \t") + 1);
    
            if (line.empty() || line[0] == '#') {
                continue;
            }
    
            std::istringstream iss(line);
            std::string prefix;
            iss >> prefix;
    
            if (prefix == "v") {
                double x, y, z;
                if (iss >> x >> y >> z) {
                    data_.vertices.push_back(x);
                    data_.vertices.push_back(y);
                    data_.vertices.push_back(z);
                    data_.vertex_count++;
                } else {
                    std::cerr << "Failed to parse vertex: " << line << std::endl;
                }
            } else if (prefix == "f") {
                std::vector<int> face;
                std::string vertex;
                while (iss >> vertex) {
                    size_t first_slash = vertex.find('/');
                    if (first_slash != std::string::npos) {
                        vertex = vertex.substr(0, first_slash);
                    }
                    try {
                        int vertex_index = std::stoi(vertex) - 1;
                        if (vertex_index < 0 || vertex_index >= data_.vertex_count) {
                            std::cerr << "Invalid vertex index: " << vertex_index + 1 << std::endl;
                            continue;
                        }
                        face.push_back(vertex_index);
                    } catch (const std::exception& e) {
                        std::cerr << "Failed to parse face index: " << vertex << std::endl;
                    }
                }
                if (face.size() >= 3) {
                    for (size_t i = 1; i < face.size() - 1; ++i) {
                        data_.edges.push_back(face[0]);
                        data_.edges.push_back(face[i]);
                        data_.edges.push_back(face[i]);
                        data_.edges.push_back(face[i + 1]);
                        data_.edge_count += 2;
                    }
                } else {
                    std::cerr << "Invalid face (less than 3 vertices): " << line << std::endl;
                }
            }
        }
    
        file.close();
        std::cout << "Loaded vertices: " << data_.vertex_count << ", edges: " << data_.edge_count << std::endl;
        notifyObservers();
    }

void ParserModel::addObserver(ModelObserver* observer) {
    observers_.push_back(observer);
    std::cout << "Observer added. Total observers: " << observers_.size() << std::endl;
}

void ParserModel::removeObserver(ModelObserver* observer) {
    observers_.erase(
        std::remove(observers_.begin(), observers_.end(), observer),
        observers_.end());
}

void ParserModel::notifyObservers() {
    std::cout << "Notifying observers of model change. Vertices: "
              << data_.vertex_count << ", Edges: " << data_.edge_count << std::endl;
    std::cout << "Number of observers: " << observers_.size() << std::endl;
    for (auto observer : observers_) {
        observer->onModelChanged(data_);
    }
}

const ObjData& ParserModel::getData() const {
    return data_;
}

void ParserModel::moveX(double value) {
    for (int i = 0; i < data_.vertex_count; ++i) {
        data_.vertices[i * 3] += value;
    }
    move_x_ += value;
    notifyObservers();
}

void ParserModel::moveY(double value) {
    for (int i = 0; i < data_.vertex_count; ++i) {
        data_.vertices[i * 3 + 1] += value;
    }
    move_y_ += value;
    notifyObservers();
}

void ParserModel::moveZ(double value) {
    for (int i = 0; i < data_.vertex_count; ++i) {
        data_.vertices[i * 3 + 2] += value;
    }
    move_z_ += value;
    notifyObservers();
}

void ParserModel::rotateX(double angle) {
    for (int i = 0; i < data_.vertex_count; ++i) {
        double y = data_.vertices[i * 3 + 1];
        double z = data_.vertices[i * 3 + 2];
        data_.vertices[i * 3 + 1] = y * cos(angle) - z * sin(angle);
        data_.vertices[i * 3 + 2] = y * sin(angle) + z * cos(angle);
    }
    rotate_x_ += angle;
    notifyObservers();
}

void ParserModel::rotateY(double angle) {
    for (int i = 0; i < data_.vertex_count; ++i) {
        double x = data_.vertices[i * 3];
        double z = data_.vertices[i * 3 + 2];
        data_.vertices[i * 3] = x * cos(angle) + z * sin(angle);
        data_.vertices[i * 3 + 2] = -x * sin(angle) + z * cos(angle);
    }
    rotate_y_ += angle;
    notifyObservers();
}

void ParserModel::rotateZ(double angle) {
    for (int i = 0; i < data_.vertex_count; ++i) {
        double x = data_.vertices[i * 3];
        double y = data_.vertices[i * 3 + 1];
        data_.vertices[i * 3] = x * cos(angle) - y * sin(angle);
        data_.vertices[i * 3 + 1] = x * sin(angle) + y * cos(angle);
    }
    rotate_z_ += angle;
    notifyObservers();
}

void ParserModel::scale(double ratio) {
    if (ratio <= 0) {
        throw std::runtime_error("Scale ratio must be positive");
    }
    for (size_t i = 0; i < data_.vertices.size(); ++i) {
        data_.vertices[i] *= ratio;
    }
    scale_ *= ratio;
    notifyObservers();
}

}  // namespace s21
