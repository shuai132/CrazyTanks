#include "ActionBar.h"

USING_NS_CC;

bool ActionBar::init() {
    _bg = Sprite::create("轮盘/背景.png");
    _tp = Sprite::create("轮盘/方向.png");
    this->addChild(_bg);
    this->addChild(_tp);

    Node::setContentSize(_bg->getContentSize() + _tp->getContentSize() / 2);
    initTouchEvent();
    return true;
}

void ActionBar::initTouchEvent() {
    _el = EventListenerTouchOneByOne::create();
    auto angleCb = [this](Touch* touch, Event* event) {
        auto pos = this->convertTouchToNodeSpaceAR(touch);
        _angle = (float)(-atan2(pos.y, pos.x) * 180 / M_PI);
        if (_angle < 0) {
            _angle += 360.f;
        }
        onAngle(_angle);

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
            onTouch(TouchEvent::START);
            angleCb(touch, event);
            return true;
        }
        return false;
    };
    _el->onTouchMoved = angleCb;
    _el->onTouchEnded = [this](Touch* touches, Event* event) {
        onTouch(TouchEvent::END);
        _tp->setPosition(Vec2::ZERO);
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_el, this);
}

float ActionBar::getAngle() {
    return _angle;
}

ActionBar::~ActionBar() {
    Director::getInstance()->getEventDispatcher()->removeEventListener(_el);
}
