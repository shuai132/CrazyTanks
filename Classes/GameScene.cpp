#include "GameScene.h"
#include "GameOverLayer.h"
#include "Food.h"

#include "ui/UIButton.h"
#include "AudioEngine.h"
#include "Utils.h"

USING_NS_CC;
using namespace utils_cc;

bool GameScene::init()
{
    if ( !Scene::init() ) {
        return false;
    }

    _wallWidth = Sprite::create("地图/1.png")->getContentSize().width;

    initMenu();

    initBg();

    initTank();

    initActionBar();

    initCtrl();

    initLogic();

    scheduleUpdate();

    return true;
}

void GameScene::onEnter() {
    Scene::onEnter();
    _bgmStart = AudioEngine::play2d("音效/start.mp3");
}

void GameScene::onExitTransitionDidStart() {
    Scene::onExitTransitionDidStart();
    AudioEngine::stop(_bgmStart);
}

void GameScene::initMenu() {
    auto btClose = ui::Button::create("按钮/退出.png");
    btClose->setPosition(tright - btClose->getContentSize()/2 - Vec2(_wallWidth, _wallWidth));
    btClose->setLocalZOrder(1000);
    btClose->addTouchEventListener([](Ref*, ui::Widget::TouchEventType type){
        if (type != ui::Widget::TouchEventType::ENDED) return;
        Director::getInstance()->end();
    });
    addChild(btClose);
}

void GameScene::initBg() {
    auto texParams = Texture2D::TexParams{
            backend::SamplerFilter::LINEAR,
            backend::SamplerFilter::LINEAR,
            backend::SamplerAddressMode::REPEAT,
            backend::SamplerAddressMode::REPEAT,
    };

    {
        // 背景
        auto bg = Sprite::create("地图/3.png", {Vec2::ZERO, visibleSize});
        bg->getTexture()->setTexParameters(texParams);
        bg->setPosition(center);
        addChild(bg);
    }

    {
        // 上下墙体
        auto wallTop = Sprite::create("地图/1.png", {0, 0, visibleSize.width, _wallWidth});
        wallTop->getTexture()->setTexParameters(texParams);
        wallTop->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        wallTop->setPosition(tleft);
        addChild(wallTop);

        auto wallBottom = Sprite::createWithSpriteFrame(wallTop->getSpriteFrame());
        wallBottom->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        wallBottom->setPosition(bleft);
        addChild(wallBottom);
    }

    {
        // 左右墙体
        auto wallLeft = Sprite::create("地图/1.png", {0, 0, _wallWidth, visibleSize.width});
        wallLeft->getTexture()->setTexParameters(texParams);
        wallLeft->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        wallLeft->setPosition(bleft);
        addChild(wallLeft);

        auto wallRight = Sprite::createWithSpriteFrame(wallLeft->getSpriteFrame());
        wallRight->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        wallRight->setPosition(tright);
        addChild(wallRight);
    }
}

void GameScene::initTank() {
    _myTank = new Tank(Tank::Type::ME);
    _myTank->setScale(0.5);
    _myTank->Angle = -90;

    addChild(_myTank);

    _myTank->setPosition(center);

    {
        auto createTTF = [](const std::string& str) {
            auto lb = Label::createWithTTF(str, "fonts/Marker Felt.ttf", 28.0f);
            lb->setColor(Color3B::WHITE);
            lb->setLocalZOrder(1000);
            return lb;
        };

        const auto padding = 10;
        {
            auto life = createTTF("Life: ");
            addChild(life);
            life->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            life->setPositionX(tleft.x + _wallWidth);
            life->setPositionY(tleft.y - _wallWidth);
            _labelLife = createTTF(std::to_string((int)_myTank->getLife()));
            _labelLife->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            _labelLife->setPositionX(life->getPositionX() + life->getContentSize().width + padding);
            _labelLife->setPositionY(life->getPositionY());
            addChild(_labelLife);
            auto lifeCb([this](float life) {
                _labelLife->setString(std::to_string((int) life));
                if (life <= 50) {
                    _labelLife->setColor(Color3B::RED);
                } else {
                    _labelLife->setColor(Color3B::WHITE);
                }
            });
            lifeCb(_myTank->getLife());
            _myTank->setLifeCb(lifeCb);
        }
        {
            auto score = createTTF("Score: ");
            addChild(score);
            score->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            score->setPositionX(tleft.x + _wallWidth);
            score->setPositionY(tleft.y - _wallWidth - score->getContentSize().height);

            _labelScore = createTTF("0");
            _labelScore->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            _labelScore->setPositionX(score->getPositionX() + score->getContentSize().width + padding);
            _labelScore->setPositionY(score->getPositionY());
            addChild(_labelScore);
        }
    }

    {
        auto animation = Animation::create();
        FOR(i, 3) {
            auto res = StringUtils::format("坦克/爆炸/%d.png", i + 1);
            animation->addSpriteFrameWithFile(res);
        }
        animation->setDelayPerUnit(1.f/3.f);
        animation->setRestoreOriginalFrame(true);
        _actionBoom = Animate::create(animation);
        _refKeeper.pushBack(_actionBoom);
    }
}

void GameScene::initActionBar() {
    _actionBar = ActionBar::create();
    addChild(_actionBar);
    _actionBar->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _actionBar->setPosition(bleft + Vec2{_wallWidth, _wallWidth} + _actionBar->getContentSize() / 2);

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
    auto fire = [this](Bullet::Type type) {
        if (_myTank->isDie()) return;
        auto bullet = new Bullet(type);
        _myTank->fire(bullet);
        bullet->setBoomCb([bullet]{
            bullet->removeFromParent();
        });
        _contact->addBullet(bullet);
    };

    {
        auto btFire = ui::Button::create("按钮/子弹.png");
        btFire->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        btFire->setPosition(Vec2{
                right - _wallWidth,
                bottom + _wallWidth
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
                right - _wallWidth,
                bottom + _wallWidth + btFire->getContentSize().height + 6
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
                right - _wallWidth,
                bottom + _wallWidth + btFire->getContentSize().height * 2 + 6
        });
        btFire->addTouchEventListener([fire](Ref*, ui::Widget::TouchEventType type){
            if (type != ui::Widget::TouchEventType::BEGAN) return;
            fire(Bullet::Type::Rock2);
        });
        addChild(btFire);
    }
}

void GameScene::initLogic() {
    _contact = new Contact(_wallWidth);
    addChild(_contact);
    _contact->addTank(_myTank);
    _contact->setUserTank(_myTank);
    AudioEngine::preload("音效/boom.mp3");
    _myTank->setDieCb([this](Bullet* lastBullet) {
        _myTank->removeFromParent();
        if (lastBullet->FromTank == _myTank) {
            log("被自己的子弹杀死了");
        }

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
    addChild(new GameOverLayer(_score, 10));
}

void GameScene::update(float delta) {
    Node::update(delta);
    updateAI(delta);
    updateFood(delta);
}

void GameScene::updateAI(float delta) {
    if (_aiNum >= 4) return;

    auto ai = new Tank(Tank::Type::AI);
    addChild(ai);
    _aiNum++;
    ai->Angle = random(0, 359);
    ai->setScale(random(0.3f, 0.6f));
    ai->setPosition(
            random(left, right),
            random(bottom, top)
    );

    _contact->addTank(ai);
    ai->setAiFireCb([this](Bullet* bullet) {
        _contact->addBullet(bullet);
    });
    ai->setDieCb([this, ai](Bullet* lastBullet) {
        ai->removeFromParent();
        _aiNum--;
        if (lastBullet->FromTank == _myTank && not _myTank->isDie()) {
            _score++;
            _labelScore->setString(std::to_string(_score));
        }

        AudioEngine::play2d("音效/boom.mp3");
        auto sp = Sprite::create();
        sp->setPosition(ai->getPosition());
        sp->setScale(ai->getScale());
        addChild(sp);
        sp->runAction(Sequence::create(
                _actionBoom->clone(),
                CallFunc::create([=]{
                    sp->removeFromParent();
                }),
                nullptr));
    });
}

void GameScene::updateFood(float delta) {
    if (_foodNum >= MAX_FOOD_NUM) return;

    auto food = Food::create();
    food->initWithFile("道具/heart.png");
    auto size = food->getContentSize();
    food->setPositionX(random(left + _wallWidth + size.width, right - _wallWidth - size.height));
    food->setPositionY(random(top - _wallWidth - size.width, bottom + _wallWidth + size.height));
    addChild(food);
    food->setEatCb([this, food](void* by) {
        food->removeFromParent();
        _foodNum--;
    });
    _foodNum++;
    _contact->addFood(food);
}
