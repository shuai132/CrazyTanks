#pragma once

#include "cocos2d.h"

#include "Tank.h"
#include "ActionBar.h"
#include "Contact.h"

class GameScene : public cocos2d::Scene
{
public:
    bool init() override;

    void initMenu();

    void initBg();

    void initTank();

    void initActionBar();

    void initCtrl();

    void initLogic();

    void gameOver();

    CREATE_FUNC(GameScene);

    void update(float delta) override;

    ~GameScene() override;

private:
    Tank* _myTank;

    ActionBar* _actionBar;

    Contact* _contact;

    Node* _particleBg;

    float wallWidth;

    uint32_t _aiNum = 0;

    uint32_t _score = 0;
};
