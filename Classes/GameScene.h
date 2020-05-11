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

private:
    Tank* _myTank;
    Tank* _aiTank;

    ActionBar* _actionBar;

    Contact* _contact;

    bool _gameOver = false;

    Node* _particleBg;
};
