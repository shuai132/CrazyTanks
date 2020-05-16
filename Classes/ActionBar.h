#pragma once

#include "cocos2d.h"
#include "MakeEvent.hpp"

class ActionBar : public cocos2d::Node {
public:
    enum class TouchEvent {
        START,
        END,
    };

public:
    CREATE_FUNC(ActionBar);

    ~ActionBar() override;

    bool init() override;

    float getAngle();

    MAKE_EVENT(Angle, float);
    MAKE_EVENT(Touch, TouchEvent);

private:
    void initTouchEvent();

private:
    cocos2d::EventListenerTouchOneByOne* _el;

    cocos2d::Sprite* _bg;    // background
    cocos2d::Sprite* _tp;    // touch point
    float _angle;
};
