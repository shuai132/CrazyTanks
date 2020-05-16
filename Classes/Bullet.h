#pragma once

#include "cocos2d.h"
#include "MakeEvent.hpp"

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

    void* FromTank = nullptr;
    uint64_t FromTime = 0;

public:
    explicit Bullet(Type type = Type::Normal);

    ~Bullet() override;

public:
    void update(float delta) override;

    bool hasBoom();

    MAKE_EVENT(Boom);

    void boom();

private:
    cocos2d::Sprite* _body;

    bool _hasBoom = false;
};
