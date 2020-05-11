#include <map>

#include "ActionBar.h"

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

    Node::setContentSize(_bg->getContentSize() + _tp->getContentSize() / 2);
    initTouchEvent();
}

void ActionBar::initTouchEvent() {
    _el = EventListenerTouchOneByOne::create();
    auto angleCb = [this](Touch* touch, Event* event) {
        auto pos = this->convertTouchToNodeSpaceAR(touch);
        _angle = (float)(-atan2(pos.y, pos.x) * 180 / M_PI);
        if (_angle < 0) {
            _angle += 360.f;
        }
        if (_angleCb) _angleCb(_angle);

        // 设置并限制位置
        auto distance = pos.distance(Vec2::ZERO);
        auto r = _bg->getContentSize().width / 2;
        if (distance <= r) {
            _tp->setPosition(pos);
        }
        else {
            _tp->setPosition(pos / distance * r);
        }
    };
    _el->onTouchBegan = [this, angleCb](Touch* touch, Event* event) {
        auto pos = this->convertTouchToNodeSpaceAR(touch);
        auto size = getContentSize();
        if (Rect(-Vec2(size / 2), size).containsPoint(pos)) {
            if (_touchEventCb) _touchEventCb(TouchEvent::START);
            angleCb(touch, event);
            return true;
        }
        return false;
    };
    _el->onTouchMoved = angleCb;
    _el->onTouchEnded = [this](Touch* touches, Event* event) {
        if (_touchEventCb) _touchEventCb(TouchEvent::END);
        _tp->setPosition(Vec2::ZERO);
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_el, this);
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

ActionBar::~ActionBar() {
    Director::getInstance()->getEventDispatcher()->removeEventListener(_el);
}
