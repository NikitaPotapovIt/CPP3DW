#include "../headers/view_settings.h"
#include "ui_settings.h"

namespace s21 {

Settings::Settings(QWidget* parent) : QDialog(parent), ui(new Ui::Settings) {
    ui->setupUi(this);

    setWindowModality(Qt::ApplicationModal);

    setStyleSheet("background-color: rgb(60, 90, 90);");

    setWindowOpacity(1.0);

    config_file_ = QDir::homePath() + "/settings.conf";
    settings_ = new QSettings(config_file_, QSettings::IniFormat);
    loadSettings();

    ui->vertex_color_r->setSuffix(" R");
    ui->vertex_color_g->setSuffix(" G");
    ui->vertex_color_b->setSuffix(" B");
    ui->edge_color_r->setSuffix(" R");
    ui->edge_color_g->setSuffix(" G");
    ui->edge_color_b->setSuffix(" B");
    ui->bg_color_r->setSuffix(" R");
    ui->bg_color_g->setSuffix(" G");
    ui->bg_color_b->setSuffix(" B");

    connect(ui->set_apply, &QPushButton::clicked, this, &Settings::applySettings);
    connect(ui->vertex_size_decr, &QPushButton::clicked, this,
            &Settings::onVertexSizeDecrClicked);
    connect(ui->vertex_size_incr, &QPushButton::clicked, this,
            &Settings::onVertexSizeIncrClicked);
    connect(ui->edge_width_decr, &QPushButton::clicked, this,
            &Settings::onEdgeWidthDecrClicked);
    connect(ui->edge_width_incr, &QPushButton::clicked, this,
            &Settings::onEdgeWidthIncrClicked);
    connect(ui->set_cancel, &QPushButton::clicked, this,
            &Settings::onSetCancelClicked);
    connect(ui->set_default, &QPushButton::clicked, this,
            &Settings::onSetDefaultClicked);
}

Settings::~Settings() {
    saveSettings();
    delete settings_;
    delete ui;
}

void Settings::loadSettings() {
    ui->vertex_color_r->setValue(settings_->value("vertex_color_r", 138).toInt());
    ui->vertex_color_g->setValue(settings_->value("vertex_color_g", 183).toInt());
    ui->vertex_color_b->setValue(settings_->value("vertex_color_b", 187).toInt());
    ui->vertex_size->setValue(settings_->value("vertex_size", 1).toInt());
    ui->vertex_shape->setCurrentIndex(
        settings_->value("vertex_shape", 0).toInt());
    ui->edge_color_r->setValue(settings_->value("edge_color_r", 138).toInt());
    ui->edge_color_g->setValue(settings_->value("edge_color_g", 183).toInt());
    ui->edge_color_b->setValue(settings_->value("edge_color_b", 187).toInt());
    ui->edge_width->setValue(settings_->value("edge_width", 1).toInt());
    ui->edge_type->setCurrentIndex(settings_->value("edge_type", 1).toInt());
    ui->bg_color_r->setValue(settings_->value("bg_color_r", 33).toInt());
    ui->bg_color_g->setValue(settings_->value("bg_color_g", 33).toInt());
    ui->bg_color_b->setValue(settings_->value("bg_color_b", 33).toInt());
    ui->projection_type->setCurrentIndex(
        settings_->value("projection_type", 1).toInt());
    ui->grid_off->setChecked(true);
}

void Settings::saveSettings() {
    settings_->setValue("vertex_color_r", ui->vertex_color_r->value());
    settings_->setValue("vertex_color_g", ui->vertex_color_g->value());
    settings_->setValue("vertex_color_b", ui->vertex_color_b->value());
    settings_->setValue("vertex_size", ui->vertex_size->value());
    settings_->setValue("vertex_shape", ui->vertex_shape->currentIndex());
    settings_->setValue("edge_color_r", ui->edge_color_r->value());
    settings_->setValue("edge_color_g", ui->edge_color_g->value());
    settings_->setValue("edge_color_b", ui->edge_color_b->value());
    settings_->setValue("edge_width", ui->edge_width->value());
    settings_->setValue("edge_type", ui->edge_type->currentIndex());
    settings_->setValue("bg_color_r", ui->bg_color_r->value());
    settings_->setValue("bg_color_g", ui->bg_color_g->value());
    settings_->setValue("bg_color_b", ui->bg_color_b->value());
    settings_->setValue("projection_type", ui->projection_type->currentIndex());
}

void Settings::setDefaultSettings() {
    settings_->setValue("vertex_color_r", 138);
    settings_->setValue("vertex_color_g", 183);
    settings_->setValue("vertex_color_b", 187);
    settings_->setValue("vertex_size", 1);
    settings_->setValue("vertex_shape", 0);
    settings_->setValue("edge_color_r", 138);
    settings_->setValue("edge_color_g", 183);
    settings_->setValue("edge_color_b", 187);
    settings_->setValue("edge_width", 1);
    settings_->setValue("edge_type", 1);
    settings_->setValue("bg_color_r", 33);
    settings_->setValue("bg_color_g", 33);
    settings_->setValue("bg_color_b", 33);
    settings_->setValue("projection_type", 1);
}

QColor Settings::getVertexColor() const {
    return QColor(ui->vertex_color_r->value(), ui->vertex_color_g->value(),
                  ui->vertex_color_b->value());
}

int Settings::getVertexSize() const { return ui->vertex_size->value(); }

int Settings::getVertexShape() const {
    return ui->vertex_shape->currentIndex();
}

QColor Settings::getEdgeColor() const {
    return QColor(ui->edge_color_r->value(), ui->edge_color_g->value(),
                  ui->edge_color_b->value());
}

int Settings::getEdgeWidth() const { return ui->edge_width->value(); }

int Settings::getEdgeType() const { return ui->edge_type->currentIndex(); }

QColor Settings::getBgColor() const {
    return QColor(ui->bg_color_r->value(), ui->bg_color_g->value(),
                  ui->bg_color_b->value());
}

bool Settings::isCentralProjection() const {
    return ui->projection_type->currentIndex() == 1;
}

void Settings::applySettings() {
    QColor vertex_color(ui->vertex_color_r->value(), ui->vertex_color_g->value(),
                        ui->vertex_color_b->value());
    int vertex_size = ui->vertex_size->value();
    int vertex_shape = ui->vertex_shape->currentIndex();
    QColor edge_color(ui->edge_color_r->value(), ui->edge_color_g->value(),
                      ui->edge_color_b->value());
    int edge_width = ui->edge_width->value();
    int edge_type = ui->edge_type->currentIndex();
    QColor bg_color(ui->bg_color_r->value(), ui->bg_color_g->value(),
                    ui->bg_color_b->value());
    bool central_projection = ui->projection_type->currentIndex() == 1;

    saveSettings();
    emit onSetApplyClicked(vertex_color, vertex_size, vertex_shape, edge_color,
                           edge_width, edge_type, bg_color, central_projection);
    accept();  // Закрываем диалог
}

void Settings::onVertexSizeDecrClicked() {
    if (ui->vertex_size->value() > 1) {
        ui->vertex_size->setValue(ui->vertex_size->value() - 1);
    }
}

void Settings::onVertexSizeIncrClicked() {
    if (ui->vertex_size->value() < 10) {
        ui->vertex_size->setValue(ui->vertex_size->value() + 1);
    }
}

void Settings::onEdgeWidthDecrClicked() {
    if (ui->edge_width->value() > 1) {
        ui->edge_width->setValue(ui->edge_width->value() - 1);
    }
}

void Settings::onEdgeWidthIncrClicked() {
    if (ui->edge_width->value() < 10) {
        ui->edge_width->setValue(ui->edge_width->value() + 1);
    }
}

void Settings::onSetCancelClicked() {
    reject();  // Закрываем диалог без сохранения
}

void Settings::onSetDefaultClicked() {
    setDefaultSettings();
    loadSettings();
}

}  // namespace s21
