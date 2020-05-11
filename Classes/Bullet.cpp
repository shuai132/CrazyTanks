#include "Bullet.h"

Bullet::Bullet(Bullet::Type type) {
    autorelease();

    _实体 = [&]{
        if (type == Bullet::Type::Normal) {
            Speed = 200;
            MaxBounceCount = 5;

            char buf[20];
            sprintf(buf, "子弹/0_%d.png", random(0, 3));
            return Sprite::create(buf);
        } else {
            Speed = 300;
            MaxBounceCount = 10;

            std::map<Bullet::Type, std::string> map = {
                    {Bullet::Type::Rock1, "子弹/1.png"},
                    {Bullet::Type::Rock2, "子弹/2.png"},
            };
            return Sprite::create(map.at(type));
        }
    }();

    this->addChild(_实体);
    scheduleUpdate();
}

Bullet::~Bullet() {
    unscheduleUpdate();
}

void Bullet::update(float delta) {
    Node::update(delta);

    auto r = (float)(getRotation() / 180 * M_PI);
    setPosition(getPosition() + delta * -Vec2{-Speed * cos(r), Speed * sin(r)});
}

bool Bullet::shouldDisappear() {
    return HasBoom or (BounceCount > MaxBounceCount);
}
