#include <gtest/gtest.h>
#include <fstream>
#include <cmath>
#include <iostream>
#include "../headers/model_parser.h"
#include "../headers/observer.h"

namespace s21 {

class TestObserver : public ModelObserver {
    public:
        TestObserver() {}
        void onModelChanged(const ObjData& data) override {
            received_data_ = data;
        }
        ObjData received_data_;
    };

TEST(ParserModelTest, LoadFileThrowsException) {
    ParserModel* model = new ParserModel();
    EXPECT_THROW(model->loadFromFile("./obj/FAKE.obj"), std::runtime_error);
    try {
        model->loadFromFile("./obj/FAKE.obj");
    } catch (const std::runtime_error& e) {
        EXPECT_STREQ(e.what(), "Unable to open file: ./obj/FAKE.obj");
    }
    delete model;
}

TEST(ParserModelTest, LoadFileSuccessfully) {
    ParserModel* model = new ParserModel();
    model->loadFromFile("./obj/cube.obj");
    const ObjData& data = model->getData();
    EXPECT_EQ(data.vertex_count, 8);
    EXPECT_EQ(data.vertices.size(), size_t(24));
    EXPECT_EQ(data.edge_count, 24);
    EXPECT_EQ(data.edges.size(), size_t(48));
    delete model;
}

TEST(ParserModelTest, MoveX) {
    ParserModel* model = new ParserModel();
    model->loadFromFile("./obj/cube.obj");
    model->moveX(1.0);
    const ObjData& data = model->getData();
    for (int i = 0; i < data.vertex_count; ++i) {
        if (i == 0 || i == 1 || i == 4 || i == 5) {
            EXPECT_NEAR(data.vertices[i * 3], 2.0, 1e-6);
        } else {
            EXPECT_NEAR(data.vertices[i * 3], 0.0, 1e-6);
        }
    }
    delete model;
}

TEST(ParserModelTest, MoveY) {
    ParserModel* model = new ParserModel();
    model->loadFromFile("./obj/cube.obj");
    model->moveY(1.0);
    const ObjData& data = model->getData();
    for (int i = 0; i < data.vertex_count; ++i) {
        if (i == 0 || i == 3 || i == 4 || i == 7) {
            EXPECT_NEAR(data.vertices[i * 3 + 1], 2.0, 1e-6);
        } else {
            EXPECT_NEAR(data.vertices[i * 3 + 1], 0.0, 1e-6);
        }
    }
    delete model;
}

TEST(ParserModelTest, MoveZ) {
    ParserModel* model = new ParserModel();
    model->loadFromFile("./obj/cube.obj");
    model->moveZ(1.0);
    const ObjData& data = model->getData();
    for (int i = 0; i < data.vertex_count; ++i) {
        if (i < 4) {
            EXPECT_NEAR(data.vertices[i * 3 + 2], 0.0, 1e-6);
        } else {
            EXPECT_NEAR(data.vertices[i * 3 + 2], 2.0, 1e-6);
        }
    }
    delete model;
}

TEST(ParserModelTest, RotateX) {
    ParserModel* model = new ParserModel();
    model->loadFromFile("./obj/cube.obj");
    model->rotateX(M_PI / 2.0);
    const ObjData& data = model->getData();
    EXPECT_NEAR(data.vertices[0 * 3 + 1], 1.0, 1e-6);   // y вершины 1
    EXPECT_NEAR(data.vertices[0 * 3 + 2], 1.0, 1e-6);   // z вершины 1
    EXPECT_NEAR(data.vertices[2 * 3 + 1], 1.0, 1e-6);   // y вершины 3
    EXPECT_NEAR(data.vertices[2 * 3 + 2], -1.0, 1e-6);  // z вершины 3
    EXPECT_NEAR(data.vertices[4 * 3 + 1], -1.0, 1e-6);  // y вершины 5
    EXPECT_NEAR(data.vertices[4 * 3 + 2], 1.0, 1e-6);   // z вершины 5
    delete model;
}

TEST(ParserModelTest, RotateY) {
    ParserModel* model = new ParserModel();
    model->loadFromFile("./obj/cube.obj");
    model->rotateY(M_PI / 2.0);
    const ObjData& data = model->getData();
    EXPECT_NEAR(data.vertices[1 * 3 + 0], -1.0, 1e-6);  // x вершины 2
    EXPECT_NEAR(data.vertices[1 * 3 + 2], -1.0, 1e-6);  // z вершины 2
    EXPECT_NEAR(data.vertices[4 * 3 + 0], 1.0, 1e-6);   // x вершины 5
    EXPECT_NEAR(data.vertices[4 * 3 + 2], -1.0, 1e-6);  // z вершины 5
    delete model;
}

TEST(ParserModelTest, RotateZ) {
    ParserModel* model = new ParserModel();
    model->loadFromFile("./obj/cube.obj");
    model->rotateZ(M_PI / 2.0);
    const ObjData& data = model->getData();
    EXPECT_NEAR(data.vertices[1 * 3 + 0], 1.0, 1e-6);   // x вершины 2
    EXPECT_NEAR(data.vertices[1 * 3 + 1], 1.0, 1e-6);   // y вершины 2
    EXPECT_NEAR(data.vertices[3 * 3 + 0], -1.0, 1e-6);  // x вершины 4
    EXPECT_NEAR(data.vertices[3 * 3 + 1], -1.0, 1e-6);  // y вершины 4
    delete model;
}

TEST(ParserModelTest, Scale) {
    ParserModel* model = new ParserModel();
    model->loadFromFile("./obj/cube.obj");
    model->scale(2.0);
    const ObjData& data = model->getData();
    for (int i = 0; i < data.vertex_count; ++i) {
        if (i == 0 || i == 1 || i == 4 || i == 5) {
            EXPECT_NEAR(data.vertices[i * 3], 2.0, 1e-6);
        } else {
            EXPECT_NEAR(data.vertices[i * 3], -2.0, 1e-6);
        }
        if (i == 0 || i == 3 || i == 4 || i == 7) {
            EXPECT_NEAR(data.vertices[i * 3 + 1], 2.0, 1e-6);
        } else {
            EXPECT_NEAR(data.vertices[i * 3 + 1], -2.0, 1e-6);
        }
        if (i < 4) {
            EXPECT_NEAR(data.vertices[i * 3 + 2], -2.0, 1e-6);
        } else {
            EXPECT_NEAR(data.vertices[i * 3 + 2], 2.0, 1e-6);
        }
    }
    delete model;
}

TEST(ParserModelTest, ScaleThrowsException) {
    ParserModel* model = new ParserModel();
    EXPECT_THROW(model->scale(0.0), std::runtime_error);
    delete model;
}

TEST(ParserModelTest, ObserverNotification) {
    ParserModel* model = new ParserModel();
    TestObserver* observer1 = new TestObserver();
    TestObserver* observer2 = new TestObserver();

    model->addObserver(observer1);
    model->addObserver(observer2);

    model->loadFromFile("./obj/cube.obj");

    EXPECT_EQ(observer1->received_data_.vertex_count, 8);
    EXPECT_EQ(observer1->received_data_.edge_count, 24);
    EXPECT_EQ(observer2->received_data_.vertex_count, 8);
    EXPECT_EQ(observer2->received_data_.edge_count, 24);

    model->removeObserver(observer1);
    model->moveX(1.0);

    EXPECT_EQ(observer1->received_data_.vertices[0], 1.0);
    EXPECT_EQ(observer2->received_data_.vertices[0], 2.0);

    delete observer1;
    delete observer2;
    delete model;
}

}  // namespace s21

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
