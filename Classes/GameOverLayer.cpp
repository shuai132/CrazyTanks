#include "ui/UIButton.h"

#include "GameOverLayer.h"
#include "GameScene.h"

USING_NS_CC;

GameOverLayer::GameOverLayer(uint32_t score) {
    Layer::init();
    autorelease();

    _bestScore = UserDefault::getInstance()->getIntegerForKey(K_BEST_SCORE);

    if (score >= _bestScore) {
        _bestScore = score;
        UserDefault::getInstance()->setIntegerForKey(K_BEST_SCORE, score);
    }

    {
        auto grayLayer = LayerColor::create(Color4B::GRAY);
        grayLayer->setOpacity(80);
        addChild(grayLayer);
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    auto center = origin + visibleSize / 2;

    {
        auto snow = ParticleSnow::create();
        snow->setPositionY(origin.y + visibleSize.height);
        addChild(snow);
    }

    {
        auto btFace = ui::Button::create(score >= 100 ? "按钮/开心.png" : "按钮/难过.png");
        addChild(btFace);
        btFace->setScale(2);
        btFace->setPositionX(center.x);
        btFace->setPositionY(center.y + 100);
        btFace->addTouchEventListener([](Ref*, ui::Widget::TouchEventType type){
            if (type != ui::Widget::TouchEventType::ENDED) return;
            Director::getInstance()->replaceScene(GameScene::create());
        });
    }

    auto createTTF = [](const std::string& str) {
        return Label::createWithTTF(str, "fonts/Marker Felt.ttf", 48.0f);
    };

    {
        auto lbScore = createTTF(std::string("SCORE: ") + std::to_string(score));
        addChild(lbScore);
        lbScore->setPositionX(center.x);
        lbScore->setPositionY(center.y - 50);
    }
    {
        auto lbBest = createTTF(std::string("BEST: ") + std::to_string(_bestScore));
        addChild(lbBest);
        lbBest->setPositionX(center.x);
        lbBest->setPositionY(center.y - 150);
    }

    {
        auto lbText = Label::createWithTTF("Click on the Head to restart", "fonts/Marker Felt.ttf", 16.0f);
        addChild(lbText);
        lbText->setPositionX(center.x);
        lbText->setPositionY(center.y - 200);
    }

    {
        _touchListener = EventListenerTouchOneByOne::create();
        _touchListener->setSwallowTouches(true);

        _touchListener->onTouchBegan = [](Touch*, Event*) {
            return true;
        };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener, this);
    }
}

GameOverLayer::~GameOverLayer() {
    _eventDispatcher->removeEventListener(_touchListener);
}
