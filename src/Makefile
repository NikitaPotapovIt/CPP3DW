CC = clang++ -std=c++20
CFLAGS = -c -Wall -Wextra -Werror -I./headers
BUILD_DIR = build
INSTALL_DIR = 3DViewer_v1_0
APP = 3dViewer_v1_0
GCOV = -fprofile-arcs -ftest-coverage
LIBCPP = ./sources/model_parser.cpp
LIBH = ./headers/model_parser.h ./headers/obj_data.h ./headers/observer.h
LIBO = ./model_parser.o
TESTCPP = ./tests/unit_tests.cpp

OS = $(shell uname)

QT_VERSION ?= 5

ifeq ($(QT_VERSION), 6)
	QMAKE = qmake6
else
	QMAKE = qmake
endif

ifeq ($(OS), Linux)
	OS_FLAGS = -lgtest -lgtest_main -pthread
	CC += -D OS_LINUX -g
	DIR =
	QTFLAGS = -spec linux-g++
	CHECK_LEAKS = valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=log.txt
	OPEN = firefox
else
	OS_FLAGS = -lgtest -lgtest_main
	CC += -D OS_MAC -g -stdlib=libc++
	DIR = /$(APP).app/Contents/MacOS/
	QTFLAGS =
	CHECK_LEAKS = leaks --atExit --
	OPEN = open
endif

SOURCES = \
	./sources/model_parser.cpp \
	./sources/view_glview.cpp \
	./sources/view_settings.cpp \
	./sources/presenter_mainviewer.cpp \
	./sources/main.cpp \
	./sources/command.cpp \
	./sources/observer.cpp

HEADERS = \
	./headers/model_parser.h \
	./headers/view_glview.h \
	./headers/view_settings.h \
	./headers/presenter_mainviewer.h \
	./headers/command.h \
	./headers/observer.h \
	./headers/obj_data.h

OBJECTS = \
	$(BUILD_DIR)/model_parser.o \
	$(BUILD_DIR)/view_glview.o \
	$(BUILD_DIR)/view_settings.o \
	$(BUILD_DIR)/presenter_mainviewer.o \
	$(BUILD_DIR)/main.o \
	$(BUILD_DIR)/command.o \
	$(BUILD_DIR)/observer.o

all: build

build: $(BUILD_DIR)/Makefile
	make -C $(BUILD_DIR)/ first

$(BUILD_DIR)/Makefile: $(APP).pro
	mkdir -p $(BUILD_DIR)
	$(QMAKE) -o $(BUILD_DIR)/Makefile ./$(APP).pro $(QTFLAGS) CONFIG+=qtquickbuilder

rebuild: clean all

install: build
	mkdir -p $(INSTALL_DIR)
ifeq ($(OS), Darwin)
	cp -r $(BUILD_DIR)/$(APP).app $(INSTALL_DIR)/
	$(BUILD_DIR)/$(APP).app/Contents/MacOS/$(APP)
else
	cp -r $(BUILD_DIR)$(DIR)$(APP) $(INSTALL_DIR)/$(APP)
	$(OPEN) $(INSTALL_DIR)/$(APP)
endif

dist: install
	tar -czvf $(INSTALL_DIR).tgz $(INSTALL_DIR)/

uninstall:
	rm -rf $(INSTALL_DIR)

parser.a: $(LIBCPP) $(LIBH)
	$(CC) $(CFLAGS) $(LIBCPP) -o $(LIBO)
	ar -rcs parser.a $(LIBO)
	ranlib $@
	cp $@ lib$@
	rm -rf *.o

tests: clean parser.a $(TESTCPP)
	$(CC) $(CFLAGS) $(TESTCPP) -o unit_tests.o
	$(CC) unit_tests.o parser.a -o Test $(OS_FLAGS)
	./Test
	rm -rf *.o *.a ./Test

gcov_report: $(LIBCPP) $(TESTCPP) $(LIBH)
	$(CC) $(CFLAGS) $(GCOV) $(LIBCPP) -c -o $(LIBO)
	$(CC) $(CFLAGS) $(GCOV) $(TESTCPP) -c -o unit_tests.o
	$(CC) $(LIBO) unit_tests.o -o Test $(OS_FLAGS) $(GCOV)
	./Test
	mkdir -p report
	gcovr -r . --html --html-details -o report/index.html
	$(OPEN) ./report/index.html
	rm -rf *.o *.gcno *.gcda Test

leaks: clean parser.a $(TESTCPP)
	$(CC) $(TESTCPP) parser.a $(OS_FLAGS) -o ./Test
	$(CHECK_LEAKS) ./Test
	rm -f *.o *.a Test

style_formatter:
	cp ../materials/linters/.clang-format ./.clang-format
	clang-format -style=Google -i $(SOURCES) $(HEADERS)
	rm -rf .clang-format

style:
	cp ../materials/linters/.clang-format ./.clang-format
	clang-format -style=Google -n $(SOURCES) $(HEADERS)
	rm -rf .clang-format

cppcheck:
	cppcheck $(SOURCES) $(HEADERS)

check: style cppcheck leaks

clean:
	rm -rf *.o *.a *.out *.txt *.gcno *.gch *.gcda *.info *.tgz *.obj report coverage \
		$(APP).pro.user Test $(BUILD_DIR) $(INSTALL_DIR) Test.dSYM ui/*.autosave

.PHONY: all build rebuild install dist uninstall tests gcov_report leaks style_formatter style cppcheck check clean parser.a
