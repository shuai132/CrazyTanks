#pragma once

#include "cocos2d.h"

class Bullet : public cocos2d::Node {
public:
    enum class Type {
        Normal,
        Rock1,
        Rock2,
    };

    float Speed = 200;

    uint32_t MaxBounceCount = 0;
    uint32_t BounceCount = 0;

    float Harm = 10;

    bool HasBoom = false;

    void* FromTank;
    uint64_t FromTime;

public:
    explicit Bullet(Type type = Type::Normal);

    ~Bullet() override;

public:
    void update(float delta) override;

    bool shouldDisappear();

private:
    cocos2d::Sprite* _实体;
};
