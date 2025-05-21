#ifndef S21_VIEW_SETTINGS_H_
#define S21_VIEW_SETTINGS_H_

#include <QDir>
#include <QSettings>
#include <QDialog>

namespace Ui {
class Settings;
}

namespace s21 {

class Settings : public QDialog {
    Q_OBJECT
private:
    Ui::Settings* ui;
    QString config_file_;
    QSettings* settings_;

public:
    explicit Settings(QWidget* parent = nullptr);
    ~Settings() override;

    QColor getVertexColor() const;
    int getVertexSize() const;
    int getVertexShape() const;
    QColor getEdgeColor() const;
    int getEdgeWidth() const;
    int getEdgeType() const;
    QColor getBgColor() const;
    bool isCentralProjection() const;

private:
    void loadSettings();
    void saveSettings();
    void setDefaultSettings();

private slots:
    void applySettings();
    void onVertexSizeDecrClicked();
    void onVertexSizeIncrClicked();
    void onEdgeWidthDecrClicked();
    void onEdgeWidthIncrClicked();
    void onSetCancelClicked();
    void onSetDefaultClicked();

signals:
    void onSetApplyClicked(QColor vertex_color, int vertex_size, int vertex_shape,
                           QColor edge_color, int edge_width, int edge_type,
                           QColor bg_color, bool central_projection);
};

}  // namespace s21

#endif  // S21_VIEW_SETTINGS_H_
