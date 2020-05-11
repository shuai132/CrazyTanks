#pragma once

#include "cocos2d.h"

#include "Tank.h"
#include "ActionBar.h"
#include "Contact.h"

class GameScene : public cocos2d::Scene
{
public:
    virtual bool init() override;

    void initMenu();

    void initBg();

    void initTank();

    void initActionBar();

    void initCtrl();

    void initLogic();

    void gameOver(bool isWin);

    CREATE_FUNC(GameScene);

    void update(float delta) override;

private:
    Tank* _myTank;
    std::vector<Tank*> _aiTanks;

    ActionBar* _actionBar;

    Contact* _contact;

    bool _gameOver = false;

    Node* _particleBg;

    float WallWidth;
};
