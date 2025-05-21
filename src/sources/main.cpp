#include <QApplication>
#include "../headers/presenter_mainviewer.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    s21::MainViewer window;
    window.show();
    return app.exec();
}
