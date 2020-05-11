#include "Tank.h"
#include "Utils.h"

#include "cocos/audio/include/AudioEngine.h"

Tank::Tank(Tank::Type type)
    : type(type), _mute(type == Type::ENEMY)
    {
    autorelease();

    _轮子 = Sprite::create("坦克/1.png");
    Node::setContentSize(_轮子->getContentSize());
    _车体 = Sprite::create("坦克/2.png");
    _炮台 = [&]{
        std::map<Tank::Type, std::string> map = {
                {Tank::Type::FRIEND, "坦克/3.png"},
                {Tank::Type::ENEMY, "坦克/3_1.png"},
        };
        return Sprite::create(map.at(type));
    }();

    {
        _发射点 = Node::create();
        _发射点->setPosition(_炮台->getContentSize().width / 2, 0);
        _炮台->addChild(_发射点);
    }
    _炮台->setAnchorPoint({0.325, 0.5});
    this->addChild(_轮子);
    this->addChild(_车体);
    this->addChild(_炮台);

    scheduleUpdate();
}

Tank::~Tank() {
    unscheduleUpdate();
}

Vec2 Tank::getFirePoint() {
    return _发射点->getPosition();
}

void Tank::update(float delta) {
    Node::update(delta);
    setRotation(Angle);
    if (isDie() || not _move) {
        if (_mute) return;
        AudioEngine::stop(_soundIdMove);
        _soundIdMove = NoSoundID;
        return;
    }

    auto r = (float)(getRotation() / 180 * M_PI);
    setPosition(getPosition() + delta * -Vec2{-Speed * cos(r), Speed * sin(r)});

    if (not _mute and _soundIdMove == NoSoundID) {
        _soundIdMove = AudioEngine::play2d("音效/move.mp3", true);
    }

    // 看起来更像运动的
    _轮子->setScale(random(0.9f, 1.f));
}

void Tank::move(bool move) {
    _move = move;
}

void Tank::fire(Bullet* bullet) {
    bullet->FromTank = this;
    bullet->FromTime = ::utils::nowMs();
    bullet->setRotation(getRotation());
    getParent()->addChild(bullet);
    bullet->setPosition(getParent()->convertToNodeSpace(convertToWorldSpace(getFirePoint())));

    AudioEngine::play2d("音效/fire.mp3");
}

void Tank::harm(Bullet* bullet) {
    _life -= bullet->Harm;
    if (_dieCb && isDie()) {
        _dieCb();
    }
}

void Tank::setDieCb(Tank::DieCb cb) {
    _dieCb = std::move(cb);
}

bool Tank::isDie() {
    return _life <= 0;
}
