# Основные модули Qt
QT += core gui widgets opengl printsupport

# Включение C++20
CONFIG += c++20
QMAKE_CXXFLAGS += -std=c++20
QMAKE_CXXFLAGS -= -std=gnu++11 -std=c++11 -std=c++0x
QMAKE_CXX = /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang++ -std=c++20

# Условная проверка версии Qt
greaterThan(QT_MAJOR_VERSION, 5) {
    QT += openglwidgets
} else {
    QT += opengl
}

# Подключение библиотеки QtGifImage
include(QtGifImage-master/src/gifimage/qtgifimage.pri)

# Исходные файлы
SOURCES += \
    sources/main.cpp \
    sources/model_parser.cpp \
    sources/view_glview.cpp \
    sources/view_settings.cpp \
    sources/presenter_mainviewer.cpp \
    sources/command.cpp \
    sources/observer.cpp

# Заголовочные файлы
HEADERS += \
    headers/model_parser.h \
    headers/view_glview.h \
    headers/view_settings.h \
    headers/presenter_mainviewer.h \
    headers/command.h \
    headers/observer.h \
    headers/obj_data.h  # Добавлен новый заголовок

# Формы UI
FORMS += \
    ui/mainviewer.ui \
    ui/settings.ui

# Пути для заголовков
INCLUDEPATH += \
    headers \
    QtGifImage-master/src/gifimage

# Правила развертывания
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

TARGET = 3dViewer_v1_0
