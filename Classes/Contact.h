#pragma once

#include "cocos2d.h"

#include "Bullet.h"
#include "Tank.h"

class Contact : public cocos2d::Node {
public:
    explicit Contact(float wallWidth);

    ~Contact() override;

    void addBullet(Bullet* bullet);

    void addTank(Tank* tank);

public:
    void update(float delta) override;

private:
    std::vector<Bullet*> _bullets;

    std::vector<Tank*> _tanks;

    float _wallWidth;
};
