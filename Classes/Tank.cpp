#include "Tank.h"
#include "Utils.h"

#include "AudioEngine.h"

USING_NS_CC;

Tank::Tank(Tank::Type type)
    : type(type), _mute(type == Type::AI), _move(type == Type::AI)
    {
    autorelease();

    _轮子 = Sprite::create("坦克/1.png");
    Node::setContentSize(_轮子->getContentSize());
    _车体 = Sprite::create("坦克/2.png");
    _炮台 = [&]{
        std::map<Tank::Type, std::string> map = {
                {Tank::Type::ME, "坦克/3.png"},
                {Tank::Type::AI, "坦克/3_1.png"},
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
    mute();
}

Vec2 Tank::getFirePoint() {
    return _发射点->getPosition();
}

void Tank::update(float delta) {
    Node::update(delta);

    if (_move && not _mute) {
        if (_soundIdMove == NoSoundID) {
            _soundIdMove = AudioEngine::play2d("音效/move.mp3", true);
        } else {
            AudioEngine::resume(_soundIdMove);
        }
    } else {
        AudioEngine::pause(_soundIdMove);
    }

    // 更新角度
    setRotation(Angle);
    // 运动
    if (_move) {
        // 看起来更像运动的
        _轮子->setScale(random(0.9f, 1.f));

        auto r = (float)(getRotation() / 180 * M_PI);
        setPosition(getPosition() + delta * -Vec2{-Speed * cos(r), Speed * sin(r)});
    }

    /// AI
    if (type == Type::AI) {
        _move = true;

        auto now = ::utils::nowMs();
        if (now - _lastTurnTime > random(2000, 3000)) {
            _lastTurnTime = now;
            Angle = random(0, 360);
        }

        if (now - _lastFireTime > random(3000, 5000)) {
            _lastFireTime = now;
            auto bullet = new Bullet(Bullet::Type::Rock1);
            bullet->setBoomCb([bullet]{
                bullet->removeFromParent();
            });
            fire(bullet);
            if (_fireCb) _fireCb(bullet);
        }
    }
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

    bullet->setScale(getScale());

    if (type != Type::AI) {
        AudioEngine::play2d("音效/fire.mp3");
    }
}

void Tank::harm(Bullet* bullet) {
    _life -= bullet->Harm;
    if (_life <= 0) {
        _life = 0;
        mute();
    }

    if (_dieCb && isDie()) {
        _dieCb(bullet);
    }

    if (_lifeCb) _lifeCb(_life);
}

void Tank::setDieCb(Tank::DieCb cb) {
    _dieCb = std::move(cb);
}

bool Tank::isDie() {
    return _life <= 0;
}

void Tank::setAiFireCb(Tank::FireCb cb) {
    _fireCb = std::move(cb);
}

float Tank::getLife() {
    return _life;
}

void Tank::setLifeCb(Tank::LifeCb cb) {
    _lifeCb = std::move(cb);
}

float Tank::addLife(float life) {
    _life += life;
    if (_lifeCb) _lifeCb(_life);
    return _life;
}

void Tank::mute() {
    if (_soundIdMove != NoSoundID) {
        AudioEngine::stop(_soundIdMove);
    }
}
