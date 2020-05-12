#pragma once

#include "cocos2d.h"

class Food : public cocos2d::Sprite {
public:
    using EatCb = std::function<void(void*)>;
    CREATE_FUNC(Food);

public:
    float Energy = 10;

public:
    void setEatCb(EatCb cb);

    void eatBy(void* any);

    bool hasEat();

private:
    EatCb _eatCb;

    bool _hasEat = false;
};
