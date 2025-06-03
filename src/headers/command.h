#ifndef COMMAND_H_
#define COMMAND_H_

#include "../headers/model_parser.h"

namespace s21 {

class Command {
public:
    virtual ~Command() {}
    virtual void execute(ParserModel& model) = 0;
};

class RotateXCommand : public Command {
public:
    RotateXCommand(double angle) : angle_(angle) {}
    void execute(ParserModel& model) override;
private:
    double angle_;
};

class RotateYCommand : public Command {
public:
    RotateYCommand(double angle) : angle_(angle) {}
    void execute(ParserModel& model) override;
private:
    double angle_;
};

class RotateZCommand : public Command {
public:
    RotateZCommand(double angle) : angle_(angle) {}
    void execute(ParserModel& model) override;
private:
    double angle_;
};

class MoveXCommand : public Command {
public:
    MoveXCommand(double value) : value_(value) {}
    void execute(ParserModel& model) override;
private:
    double value_;
};

class MoveYCommand : public Command {
public:
    MoveYCommand(double value) : value_(value) {}
    void execute(ParserModel& model) override;
private:
    double value_;
};

class MoveZCommand : public Command {
public:
    MoveZCommand(double value) : value_(value) {}
    void execute(ParserModel& model) override;
private:
    double value_;
};

class ScaleCommand : public Command {
public:
    ScaleCommand(double ratio) : ratio_(ratio) {}
    void execute(ParserModel& model) override;
private:
    double ratio_;
};

}  // namespace s21

#endif  // COMMAND_H_
