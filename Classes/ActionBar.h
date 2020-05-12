#pragma once

#include "cocos2d.h"

class ActionBar : public cocos2d::Node {
public:
    enum class TouchEvent {
        START,
        END,
    };

    using AngleCb = std::function<void(float)>;
    using TouchEventCb = std::function<void(TouchEvent)>;

public:
    CREATE_FUNC(ActionBar);

    ~ActionBar() override;

    bool init() override;

    float getAngle();

    void setAngleCb(AngleCb cb);

    void setTouchEventCb(TouchEventCb cb);

private:
    void initTouchEvent();

private:
    cocos2d::EventListenerTouchOneByOne* _el;

    cocos2d::Sprite* _bg;    // background
    cocos2d::Sprite* _tp;    // touch point
    float _angle;
    AngleCb _angleCb;
    TouchEventCb _touchEventCb;
};
