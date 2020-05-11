#pragma once

#include "cocos2d.h"

#include "Bullet.h"

USING_NS_CC;

class Tank : public Node {
public:
    enum Type {
        FRIEND,
        ENEMY,
    };

    using DieCb = std::function<void()>;

public:
    float Speed = 200;
    float Angle = 0.f;

public:
    explicit Tank(Type type = FRIEND);
    ~Tank() override;

    void update(float delta) override;

public:
    Vec2 getFirePoint();

    void move(bool move);

    void fire(Bullet* bullet);

    void harm(Bullet* bullet);

    void setDieCb(DieCb cb);

    bool isDie();

private:
    Type type;

    Sprite* _轮子;
    Sprite* _车体;
    Sprite* _炮台;

    Node* _发射点;
    bool _move = false;

    static const auto NoSoundID = -1;
    int _soundIdMove = NoSoundID;

    float _life = 10.f;
    DieCb _dieCb;

    bool _mute;
};
