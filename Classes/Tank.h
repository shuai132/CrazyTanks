#pragma once

#include "cocos2d.h"
#include "AudioEngine.h"

#include "Bullet.h"
#include "MakeEvent.hpp"

class Tank : public cocos2d::Node {
public:
    enum Type {
        ME,
        AI,
    };

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

    bool isDie();

    float getLife();

    float addLife(float life);

    void mute();

public:
    MAKE_EVENT(Die, Bullet* lastBullet);
    MAKE_EVENT(AiFire, Bullet*);
    MAKE_EVENT(Life, float lifeNow);

private:
    Type type;

    cocos2d::Sprite* _wheel;
    cocos2d::Sprite* _body;
    cocos2d::Sprite* _gun;

    Node* _firePoint;
    bool _move = false;

    const int& NoSoundID = cocos2d::AudioEngine::INVALID_AUDIO_ID;
    int _soundIdMove = NoSoundID;

    float _life = 10;

    bool _mute;

    // AI
    uint64_t _lastTurnTime = 0;
    uint64_t _lastFireTime = 0;
};
