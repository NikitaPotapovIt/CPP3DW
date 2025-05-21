#ifndef S21_VIEW_GLVIEW_H_
#define S21_VIEW_GLVIEW_H_

#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include "./model_parser.h"
#include "./command.h"
#include "observer.h"

namespace s21 {

class GLView : public QOpenGLWidget, protected QOpenGLFunctions, public ModelObserver {
    Q_OBJECT
private:
    ObjData data_;
    QColor vertex_color_;
    int vertex_size_;
    int vertex_shape_;
    QColor edge_color_;
    int edge_width_;
    int edge_type_;
    QColor bg_color_;
    bool central_projection_;
    QPoint last_pos_;
    ParserModel* model_;

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

public:
    explicit GLView(QWidget* parent = nullptr);
    explicit GLView(ParserModel& model, QWidget* parent = nullptr);
    ~GLView() override = default;

    void setModel(ParserModel& model);
    void setData(const ObjData& data);
    void applySettings(QColor vertex_color, int vertex_size, int vertex_shape,
                       QColor edge_color, int edge_width, int edge_type,
                       QColor bg_color, bool central_projection);
    void updateAnimation(float angle);

    void onModelChanged(const ObjData& data) override;

signals:
    void requestFrame(QImage frame);
    void rotationChanged(float x, float y);
};

}  // namespace s21

#endif  // S21_VIEW_GLVIEW_H_
