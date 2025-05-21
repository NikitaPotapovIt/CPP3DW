#ifndef S21_PRESENTER_MAINVIEWER_H_
#define S21_PRESENTER_MAINVIEWER_H_

#include <QImage>
#include <QMainWindow>
#include <QTimer>
#include <vector>
#include <memory>
#include "../headers/model_parser.h"
#include "../headers/observer.h"
#include "../headers/view_glview.h"
#include "../headers/view_settings.h"
#include "../headers/command.h"
#include "QtGifImage-master/src/gifimage/qgifimage.h"

namespace Ui {
class MainViewer;  // Предварительное объявление для UI
}

namespace s21 {

class MainViewer : public QMainWindow {
    Q_OBJECT
private:
    Ui::MainViewer* ui;
    ParserModel model_;
    GLView* gl_widget_;
    Settings* settings_dialog_;
    QTimer* animation_timer_;
    QGifImage* gif_;
    std::vector<QImage> gif_frames_;
    std::vector<std::unique_ptr<Command>> animation_commands_;
    size_t current_command_index_;
    QString gif_file_path_;

public:
    explicit MainViewer(QWidget* parent = nullptr);
    ~MainViewer() override;

private slots:
    void loadFile();
    void onTranslate();
    void onRotate();
    void onScale();
    void openSettings();
    void startAnimation();
    void saveScreenshot();
    void saveGif();
    void updateAnimation();
    void captureFrame(QImage frame);
    void updateRotationUI(float x, float y);
};

}  // namespace s21

#endif  // S21_PRESENTER_MAINVIEWER_H_
