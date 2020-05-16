#include "Food.h"

void Food::eatBy(void* any) {
    if (_hasEat) return;
    _hasEat = true;
    onEat(any);
}

bool Food::hasEat() {
    return _hasEat;
}
