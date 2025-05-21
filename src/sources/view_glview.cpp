#include "../headers/view_glview.h"
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QPainter>
#include <QWheelEvent>
#include <iostream>

namespace s21 {

GLView::GLView(QWidget* parent)
    : QOpenGLWidget(parent),
      vertex_color_(138, 183, 187),
      vertex_size_(1),
      vertex_shape_(0),
      edge_color_(138, 183, 187),
      edge_width_(1),
      edge_type_(1),
      bg_color_(33, 33, 33),
      central_projection_(true),
      model_(nullptr) {}

GLView::GLView(ParserModel& model, QWidget* parent)
    : QOpenGLWidget(parent),
      vertex_color_(138, 183, 187),
      vertex_size_(1),
      vertex_shape_(0),
      edge_color_(138, 183, 187),
      edge_width_(1),
      edge_type_(1),
      bg_color_(33, 33, 33),
      central_projection_(true),
      model_(&model) {
    model_->addObserver(this);
}

void GLView::setModel(ParserModel& model) {
    if (model_) {
        model_->removeObserver(this);
    }
    model_ = &model;
    model_->addObserver(this);
}

void GLView::initializeGL() {
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POINT_SMOOTH);
}

void GLView::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (central_projection_) {
        QMatrix4x4 proj;
        proj.perspective(45.0f, static_cast<float>(w) / h, 0.1f, 1000.0f);
        glLoadMatrixf(proj.constData());
    } else {
        glOrtho(-10, 10, -10, 10, -100, 100);
    }
    glMatrixMode(GL_MODELVIEW);
}

void GLView::paintGL() {
    glClearColor(bg_color_.redF(), bg_color_.greenF(), bg_color_.blueF(), 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    if (central_projection_) {
        glTranslatef(0.0f, 0.0f, -20.0f);
    }

    glLineWidth(edge_width_);
    if (edge_type_ == 1) {
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(1, 0x00FF);
    } else {
        glDisable(GL_LINE_STIPPLE);
    }
    glBegin(GL_LINES);
    glColor3f(edge_color_.redF(), edge_color_.greenF(), edge_color_.blueF());
    for (size_t i = 0; i < data_.edges.size(); i += 2) {
        int v1 = data_.edges[i];
        int v2 = data_.edges[i + 1];
        glVertex3d(data_.vertices[v1 * 3], data_.vertices[v1 * 3 + 1],
                   data_.vertices[v1 * 3 + 2]);
        glVertex3d(data_.vertices[v2 * 3], data_.vertices[v2 * 3 + 1],
                   data_.vertices[v2 * 3 + 2]);
    }
    glEnd();
    glDisable(GL_LINE_STIPPLE);

    // Отрисовка вершин
    if (vertex_shape_ != 0) {
        glPointSize(vertex_size_);
        glBegin(GL_POINTS);
        glColor3f(vertex_color_.redF(), vertex_color_.greenF(),
                  vertex_color_.blueF());
        for (int i = 0; i < data_.vertex_count; i++) {
            glVertex3d(data_.vertices[i * 3], data_.vertices[i * 3 + 1],
                       data_.vertices[i * 3 + 2]);
        }
        glEnd();

        if (vertex_shape_ == 2) {
            glBegin(GL_QUADS);
            glColor3f(vertex_color_.redF(), vertex_color_.greenF(),
                      vertex_color_.blueF());
            float size = vertex_size_ / 100.0f;
            for (int i = 0; i < data_.vertex_count; i++) {
                double x = data_.vertices[i * 3];
                double y = data_.vertices[i * 3 + 1];
                double z = data_.vertices[i * 3 + 2];
                glVertex3d(x - size, y - size, z);
                glVertex3d(x + size, y - size, z);
                glVertex3d(x + size, y + size, z);
                glVertex3d(x - size, y + size, z);
            }
            glEnd();
        }
    }

    QImage frame = grabFramebuffer();
    emit requestFrame(frame);
}

void GLView::setData(const ObjData& data) {
    data_ = data;
    update();
}

void GLView::applySettings(QColor vertex_color, int vertex_size, int vertex_shape,
                           QColor edge_color, int edge_width, int edge_type,
                           QColor bg_color, bool central_projection) {
    vertex_color_ = vertex_color;
    vertex_size_ = vertex_size;
    vertex_shape_ = vertex_shape;
    edge_color_ = edge_color;
    edge_width_ = edge_width;
    edge_type_ = edge_type;
    bg_color_ = bg_color;
    central_projection_ = central_projection;
    resizeGL(width(), height());
    update();
}

void GLView::updateAnimation(float angle) {
    if (model_) {
        RotateYCommand cmd(angle * M_PI / 180.0);
        cmd.execute(*model_);
    }
}

void GLView::onModelChanged(const ObjData& data) {
    data_ = data;
    update();
}

void GLView::mousePressEvent(QMouseEvent* event) {
    last_pos_ = event->pos();
}

void GLView::mouseMoveEvent(QMouseEvent* event) {
    if (model_ && (event->buttons() & Qt::LeftButton)) {
        float dx = (event->x() - last_pos_.x()) * 0.01f;
        float dy = (event->y() - last_pos_.y()) * 0.01f;
        std::cout << "Mouse rotation: dx=" << dx << ", dy=" << dy << std::endl;

        RotateYCommand cmdY(dx);
        RotateXCommand cmdX(dy);
        cmdY.execute(*model_);
        cmdX.execute(*model_);

        last_pos_ = event->pos();

        float rot_x = model_->getRotateX() * 180.0 / M_PI;
        float rot_y = model_->getRotateY() * 180.0 / M_PI;
        emit rotationChanged(rot_x, rot_y);
    }
}

void GLView::wheelEvent(QWheelEvent* event) {
    if (model_) {
        float delta = event->angleDelta().y() > 0 ? 1.1f : 0.9f;
        std::cout << "Wheel scale: delta=" << delta << std::endl;
        ScaleCommand cmd(delta);
        cmd.execute(*model_);
    }
}

}  // namespace s21
