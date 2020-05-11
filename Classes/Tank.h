#pragma once

#include "cocos2d.h"

#include "Bullet.h"

class Tank : public cocos2d::Node {
public:
    enum Type {
        ME,
        AI,
    };

    using DieCb = std::function<void(Bullet* lastBullet)>;
    using FireCb = std::function<void(Bullet*)>;
    using LifeCb = std::function<void(float life)>;

public:
    float Speed = 200;
    float Angle = 0.f;

public:
    explicit Tank(Type type = ME);
    ~Tank() override;

    void update(float delta) override;

public:
    cocos2d::Vec2 getFirePoint();

    void move(bool move);

    void fire(Bullet* bullet);

    void harm(Bullet* bullet);

    void setDieCb(DieCb cb);

    bool isDie();

    void setAiFireCb(FireCb cb);

    float getLife();

    float addLife(float life);

    void setLifeCb(LifeCb cb);

private:
    Type type;

    cocos2d::Sprite* _轮子;
    cocos2d::Sprite* _车体;
    cocos2d::Sprite* _炮台;

    Node* _发射点;
    bool _move = false;

    static const auto NoSoundID = -1;
    int _soundIdMove = NoSoundID;

    float _life;
    DieCb _dieCb;

    bool _mute;

    // AI
    uint64_t _lastTurnTime = 0;
    uint64_t _lastFireTime = 0;

    FireCb _fireCb;

    LifeCb _lifeCb
    ;
};
