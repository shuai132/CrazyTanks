#include "Food.h"

void Food::setEatCb(EatCb cb) {
    _eatCb = std::move(cb);
}

void Food::eatBy(void* any) {
    if (_hasEat) return;
    _hasEat = true;
    if (_eatCb) _eatCb(any);
}

bool Food::hasEat() {
    return _hasEat;
}
