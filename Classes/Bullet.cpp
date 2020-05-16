#include "Bullet.h"

USING_NS_CC;

Bullet::Bullet(Bullet::Type type) {
    autorelease();

    _body = [&] {
        if (type == Bullet::Type::Rock2) {
            Speed = 300;
            Harm = 50;
            MaxBounceCount = 2;
            return Sprite::create("子弹/1.png");
        } else if (type == Bullet::Type::Rock1) {
            Speed = 300;
            Harm = 10;
            MaxBounceCount = 0;
            return Sprite::create("子弹/2.png");
        } else {
            Speed = 260;
            Harm = 10;
            MaxBounceCount = 0;

            auto res = StringUtils::format("子弹/0_%d.png", random(0, 3));
            return Sprite::create(res);
        }
    }();

    this->addChild(_body);
    scheduleUpdate();

    _hasBoom = false;
}

Bullet::~Bullet() {
    unscheduleUpdate();
}

void Bullet::update(float delta) {
    Node::update(delta);

    auto r = (float)(getRotation() / 180 * M_PI);
    setPosition(getPosition() + delta * -Vec2{-Speed * cos(r), Speed * sin(r)});

    if (BounceCount > MaxBounceCount) {
        boom();
    }
}

bool Bullet::hasBoom() {
    return _hasBoom;
}

void Bullet::boom() {
    if (_hasBoom) return;
    _hasBoom = true;
    onBoom();
}
