#ifndef OBSERVER_H
#define OBSERVER_H

#include "model_parser.h"

namespace s21 {

class ModelObserver {
public:
    virtual ~ModelObserver() = default;
    virtual void onModelChanged(const ObjData& data) = 0;
};

}  // namespace s21

#endif  // OBSERVER_H
