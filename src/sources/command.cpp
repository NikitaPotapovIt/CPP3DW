#include "../headers/command.h"
#include <iostream>

namespace s21 {

void RotateXCommand::execute(ParserModel& model) {
    std::cout << "Executing RotateXCommand with angle: " << angle_ << std::endl;
    model.rotateX(angle_);
}

void RotateYCommand::execute(ParserModel& model) {
    std::cout << "Executing RotateYCommand with angle: " << angle_ << std::endl;
    model.rotateY(angle_);
}

void RotateZCommand::execute(ParserModel& model) {
    std::cout << "Executing RotateZCommand with angle: " << angle_ << std::endl;
    model.rotateZ(angle_);
}

void MoveXCommand::execute(ParserModel& model) {
    std::cout << "Executing MoveXCommand with value: " << value_ << std::endl;
    model.moveX(value_);
}

void MoveYCommand::execute(ParserModel& model) {
    std::cout << "Executing MoveYCommand with value: " << value_ << std::endl;
    model.moveY(value_);
}

void MoveZCommand::execute(ParserModel& model) {
    std::cout << "Executing MoveZCommand with value: " << value_ << std::endl;
    model.moveZ(value_);
}

void ScaleCommand::execute(ParserModel& model) {
    std::cout << "Executing ScaleCommand with ratio: " << ratio_ << std::endl;
    model.scale(ratio_);
}

}  // namespace s21
