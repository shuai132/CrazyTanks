#include "GameScene.h"

#include "ui/UIButton.h"
#include "cocos/audio/include/AudioEngine.h"

USING_NS_CC;

bool GameScene::init()
{
    if ( !Scene::init() ) {
        return false;
    }

    WallWidth = Sprite::create("地图/1.png")->getContentSize().width;

    initMenu();

    initBg();

    initTank();

    initActionBar();

    initCtrl();

    initLogic();

    AudioEngine::play2d("音效/start.mp3");

    return true;
}

void GameScene::initMenu() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    auto btClose = ui::Button::create("按钮/退出.png");
    btClose->setPosition(origin + visibleSize - btClose->getContentSize()/2 - Vec2(WallWidth, WallWidth));
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
        auto wallTop = Sprite::create("地图/1.png", Rect(0, 0, visibleSize.width, WallWidth));
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
        auto wallLeft = Sprite::create("地图/1.png", Rect(0, 0, WallWidth, visibleSize.width));
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

    _myTank = new Tank(Tank::Type::FRIEND);
    _aiTank = new Tank(Tank::Type::ENEMY);

    _myTank->setScale(0.5);
    _aiTank->setScale(0.5);

    addChild(_myTank);
    addChild(_aiTank);

    _myTank->setPosition(
            origin.x + _myTank->getContentSize().width,
            origin.y + visibleSize.height / 2
            );

    _aiTank->setPosition(
            origin.x + visibleSize.width - _aiTank->getContentSize().width,
            origin.y + visibleSize.height / 2
            );
    _aiTank->Angle = 180;
}

void GameScene::initActionBar() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    _actionBar = new ActionBar(ActionBar::Type::LEFT);
    addChild(_actionBar);
    _actionBar->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _actionBar->setPosition(origin + Vec2{WallWidth, WallWidth} + _actionBar->getContentSize() / 2);

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
                origin.x + visibleSize.width - WallWidth,
                origin.y + WallWidth
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
                origin.x + visibleSize.width - WallWidth,
                origin.y + WallWidth + btFire->getContentSize().height + 6
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
                origin.x + visibleSize.width - WallWidth,
                origin.y + WallWidth + btFire->getContentSize().height * 2 + 6
        });
        btFire->addTouchEventListener([fire](Ref*, ui::Widget::TouchEventType type){
            if (type != ui::Widget::TouchEventType::BEGAN) return;
            fire(Bullet::Type::Rock2);
        });
        addChild(btFire);
    }
}

void GameScene::initLogic() {
    _contact = new Contact(WallWidth);
    addChild(_contact);
    _contact->addTank(_myTank);
    _contact->addTank(_aiTank);
    AudioEngine::preload("音效/boom.mp3");
    _myTank->setDieCb([this]{
        AudioEngine::play2d("音效/boom.mp3");
        if (_gameOver) return;
        gameOver(false);
    });
    _aiTank->setDieCb([this]{
        AudioEngine::play2d("音效/boom.mp3");
        if (_gameOver) return;
        gameOver(true);
    });
}

void GameScene::gameOver(bool isWin) {
    _gameOver = true;
    if (isWin) {
        removeChild(_particleBg);
        auto p = ParticleSnow::create();
        addChild(p);
    }
    auto bt = ui::Button::create(isWin ? "按钮/开心.png" : "按钮/难过.png");
    bt->setScale(3);
    bt->addTouchEventListener([this, bt](Ref*, ui::Widget::TouchEventType type){
        if (type != ui::Widget::TouchEventType::BEGAN) return;
        bt->removeFromParent();

        Director::getInstance()->replaceScene(GameScene::create());
    });
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    bt->setPosition(origin + visibleSize / 2);
    addChild(bt);
}
