#include "ActionBar.h"

#include <map>

ActionBar::ActionBar(ActionBar::Type type) {
    autorelease();

    _bg = Sprite::create("轮盘/背景.png");
    _tp = [&]{
        std::map<ActionBar::Type, std::string> map = {
                {ActionBar::Type::LEFT, "轮盘/方向.png"},
                {ActionBar::Type::RIGHT, "轮盘/瞄准.png"},
        };
        return Sprite::create(map.at(type));
    }();
    this->addChild(_bg);
    this->addChild(_tp);

    Node::setContentSize(_bg->getContentSize());
    initTouchEvent();
}

void ActionBar::initTouchEvent() {
    Point posOrigin = _tp->getPosition();
    auto el = EventListenerTouchAllAtOnce::create();
    auto angleCb = [this, posOrigin](const std::vector<Touch*>& touches, Event* event) {
        Touch* touch;
        for (const auto& t : touches) {
            touch = t;
            break;//todo
        }

        auto posNow = touch->getLocation() - this->getPosition();
        _tp->setPosition(posNow);

        auto diff = posNow - posOrigin;
        _angle = (float)(-atan2(diff.y, diff.x) * 180 / M_PI);
        if (_angle < 0) {
            _angle += 360.f;
        }
        if (_angleCb) _angleCb(_angle);
    };
    el->onTouchesBegan = [this, angleCb](const std::vector<Touch*>& touches, Event* event) {
        if (_touchEventCb) _touchEventCb(TouchEvent::START);
        angleCb(touches, event);
    };
    el->onTouchesMoved = angleCb;
    el->onTouchesEnded = [this, posOrigin](const std::vector<Touch*>& touches, Event* event) {
        if (_touchEventCb) _touchEventCb(TouchEvent::END);
        _tp->setPosition(posOrigin);
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(el, this);
}

float ActionBar::getAngle() {
    return _angle;
}

void ActionBar::setAngleCb(ActionBar::AngleCb cb) {
    _angleCb = std::move(cb);
}

void ActionBar::setTouchEventCb(ActionBar::TouchEventCb cb) {
    _touchEventCb = std::move(cb);
}
