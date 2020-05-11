#include "GameScene.h"
#include "GameOverLayer.h"

#include "ui/UIButton.h"
#include "cocos/audio/include/AudioEngine.h"
#include "Utils.h"

USING_NS_CC;

bool GameScene::init()
{
    if ( !Scene::init() ) {
        return false;
    }

    wallWidth = Sprite::create("地图/1.png")->getContentSize().width;

    initMenu();

    initBg();

    initTank();

    initActionBar();

    initCtrl();

    initLogic();

    AudioEngine::play2d("音效/start.mp3");

    scheduleUpdate();

    return true;
}

void GameScene::initMenu() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    auto btClose = ui::Button::create("按钮/退出.png");
    btClose->setPosition(origin + visibleSize - btClose->getContentSize()/2 - Vec2(wallWidth, wallWidth));
    btClose->setLocalZOrder(1000);
    btClose->addTouchEventListener([](Ref*, ui::Widget::TouchEventType type){
        if (type != ui::Widget::TouchEventType::ENDED) return;
        Director::getInstance()->end();
    });
    addChild(btClose);
}

void GameScene::initBg() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    auto texParams = Texture2D::TexParams{
            backend::SamplerFilter::LINEAR,
            backend::SamplerFilter::LINEAR,
            backend::SamplerAddressMode::REPEAT,
            backend::SamplerAddressMode::REPEAT,
    };

    {
        // 背景
        auto bg = Sprite::create("地图/3.png", Rect(0, 0, visibleSize.width, visibleSize.height));
        bg->getTexture()->setTexParameters(texParams);
        bg->setPosition(origin + visibleSize / 2);
        addChild(bg);
    }

    {
        // 上下墙体
        auto wallTop = Sprite::create("地图/1.png", Rect(0, 0, visibleSize.width, wallWidth));
        wallTop->getTexture()->setTexParameters(texParams);
        wallTop->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        wallTop->setPosition(origin.x, origin.y + visibleSize.height);
        addChild(wallTop);

        auto wallBottom = Sprite::createWithSpriteFrame(wallTop->getSpriteFrame());
        wallBottom->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        wallBottom->setPosition(origin);
        addChild(wallBottom);
    }

    {
        // 左右墙体
        auto wallLeft = Sprite::create("地图/1.png", Rect(0, 0, wallWidth, visibleSize.width));
        wallLeft->getTexture()->setTexParameters(texParams);
        wallLeft->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        wallLeft->setPosition(origin);
        addChild(wallLeft);

        auto wallRight = Sprite::createWithSpriteFrame(wallLeft->getSpriteFrame());
        wallRight->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        wallRight->setPosition(origin + visibleSize);
        addChild(wallRight);
    }

    {
        _particleBg = ParticleRain::create();
        addChild(_particleBg);
    }
}

void GameScene::initTank() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    auto center = origin + visibleSize / 2;

    _myTank = new Tank(Tank::Type::ME);
    _myTank->setScale(0.5);

    addChild(_myTank);

    _myTank->setPosition(center);
}

void GameScene::initActionBar() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    _actionBar = new ActionBar(ActionBar::Type::LEFT);
    addChild(_actionBar);
    _actionBar->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _actionBar->setPosition(origin + Vec2{wallWidth, wallWidth} + _actionBar->getContentSize() / 2);

    _actionBar->setAngleCb([this](float angle) {
        _myTank->Angle = angle;
    });

    _actionBar->setTouchEventCb([this](ActionBar::TouchEvent event) {
        switch (event) {
            case ActionBar::TouchEvent::START:
            {
                _myTank->move(true);
            }
                break;
            case ActionBar::TouchEvent::END:
            {
                _myTank->move(false);
            }
                break;
        }
    });
}

void GameScene::initCtrl() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    auto fire = [this](Bullet::Type type) {
        if (_myTank->isDie()) return;
        auto bullet = new Bullet(type);
        _myTank->fire(bullet);
        _contact->addBullet(bullet);
    };

    {
        auto btFire = ui::Button::create("按钮/子弹.png");
        btFire->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        btFire->setPosition(Vec2{
                origin.x + visibleSize.width - wallWidth,
                origin.y + wallWidth
        });
        btFire->addTouchEventListener([fire](Ref*, ui::Widget::TouchEventType type){
            if (type != ui::Widget::TouchEventType::BEGAN) return;
            fire(Bullet::Type::Normal);
        });
        addChild(btFire);
    }
    {
        auto btFire = ui::Button::create("按钮/导弹.png");
        btFire->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        btFire->setPosition(Vec2{
                origin.x + visibleSize.width - wallWidth,
                origin.y + wallWidth + btFire->getContentSize().height + 6
        });
        btFire->addTouchEventListener([fire](Ref*, ui::Widget::TouchEventType type){
            if (type != ui::Widget::TouchEventType::BEGAN) return;
            fire(Bullet::Type::Rock1);
        });
        addChild(btFire);
    }
    {
        auto btFire = ui::Button::create("按钮/导弹.png");
        btFire->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        btFire->setPosition(Vec2{
                origin.x + visibleSize.width - wallWidth,
                origin.y + wallWidth + btFire->getContentSize().height * 2 + 6
        });
        btFire->addTouchEventListener([fire](Ref*, ui::Widget::TouchEventType type){
            if (type != ui::Widget::TouchEventType::BEGAN) return;
            fire(Bullet::Type::Rock2);
        });
        addChild(btFire);
    }
}

void GameScene::initLogic() {
    _contact = new Contact(wallWidth);
    addChild(_contact);
    _contact->addTank(_myTank);
    AudioEngine::preload("音效/boom.mp3");
    _myTank->setDieCb([this]{
        _myTank->setVisible(false);

        AudioEngine::play2d("音效/boom.mp3");
        auto p = ParticleExplosion::create();
        p->setPosition(_myTank->getPosition());
        p->setSpeed(p->getSpeed() / 2);
        addChild(p);

        gameOver();
    });
}

void GameScene::gameOver() {
    unscheduleUpdate();
    stopAllActions();
    addChild(new GameOverLayer(_score));
}

void GameScene::update(float delta) {
    Node::update(delta);

    if (_aiNum >= 4) return;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    auto ai = new Tank(Tank::Type::AI);
    addChild(ai);
    _aiNum++;
    ai->Angle = random(0, 359);
    ai->setScale(random(0.3f, 0.6f));
    ai->setPosition(
            random(origin.x, origin.x + visibleSize.width),
            random(origin.y, origin.y + visibleSize.height)
    );

    _contact->addTank(ai);
    ai->setOnAiFireCb([this](Bullet* bullet) {
        _contact->addBullet(bullet);
    });
    ai->setDieCb([this, ai]{
        _aiNum--;
        _score++;
        AudioEngine::play2d("音效/boom.mp3");
        auto animation = Animation::create();
        FOR(i, 3) {
            char path[20];
            sprintf(path, "坦克/爆炸/%d.png", i + 1);
            animation->addSpriteFrameWithFile(path);
        }
        animation->setDelayPerUnit(1.f/3.f);
        animation->setRestoreOriginalFrame(true);
        auto action = Animate::create(animation);
        auto sp = Sprite::create();
        sp->setPosition(ai->getPosition());
        addChild(sp);
        sp->runAction(Sequence::create(
                action,
                CallFunc::create([=]{
                    sp->removeFromParent();
                }),
                nullptr));
    });
}

GameScene::~GameScene() {
    AudioEngine::stopAll();
}
