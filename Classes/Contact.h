#pragma once

#include "cocos2d.h"

#include "Bullet.h"
#include "Tank.h"
#include "Food.h"

class Contact : public cocos2d::Node {
public:
    explicit Contact(float wallWidth);

    ~Contact() override;

    void addBullet(Bullet* bullet);

    void addTank(Tank* tank);

    void setUserTank(Tank* tank);

    void addFood(Food* food);

public:
    void update(float delta) override;

    void updateBullet(float delta);

    void updateTank(float delta);

    void updateFood(float food);

private:
    cocos2d::Vector<Bullet*> _bullets;

    cocos2d::Vector<Tank*> _tanks;

    Tank* _userTank;

    cocos2d::Vector<Food*> _foods;

    float _wallWidth;
};
