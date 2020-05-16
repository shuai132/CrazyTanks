#pragma once

#include "cocos2d.h"
#include "MakeEvent.hpp"

class Food : public cocos2d::Sprite {
public:
    CREATE_FUNC(Food);

public:
    float Energy = 1;

public:
    MAKE_EVENT(Eat, void*);

    void eatBy(void* any);

    bool hasEat();

private:
    bool _hasEat = false;
};
