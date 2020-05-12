#pragma once

#include "cocos2d.h"

class Bullet : public cocos2d::Node {
public:
    enum class Type {
        Normal,
        Rock1,
        Rock2,
    };

    using BoomCb = std::function<void()>;

    float Speed = 200;

    uint32_t MaxBounceCount = 0;
    uint32_t BounceCount = 0;

    float Harm = 10;

    void* FromTank;
    uint64_t FromTime;

public:
    explicit Bullet(Type type = Type::Normal);

    ~Bullet() override;

public:
    void update(float delta) override;

    bool hasBoom();

    void setBoomCb(BoomCb cb);

    void boom();

private:
    cocos2d::Sprite* _实体;

    BoomCb _boomCb;

    bool _hasBoom = false;
};
