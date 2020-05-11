#pragma once

#include "cocos2d.h"

USING_NS_CC;

class ActionBar : public Node {
public:
    enum class Type {
        LEFT,
        RIGHT,
    };

    enum class TouchEvent {
        START,
        END,
    };

    using AngleCb = std::function<void(float)>;
    using TouchEventCb = std::function<void(TouchEvent)>;

public:
    explicit ActionBar(Type type);

    ~ActionBar() override;

    float getAngle();

    void setAngleCb(AngleCb cb);

    void setTouchEventCb(TouchEventCb cb);

private:
    void initTouchEvent();

private:
    EventListenerTouchOneByOne* _el;

    Sprite* _bg;    // background
    Sprite* _tp;    // touch point
    float _angle;
    AngleCb _angleCb;
    TouchEventCb _touchEventCb;
};
