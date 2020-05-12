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

    void updateAI(float delta);

    void updateFood(float delta);

    ~GameScene() override;

private:
    Tank* _myTank;

    ActionBar* _actionBar;

    Contact* _contact;

    float wallWidth;

    uint32_t _aiNum = 0;

    uint32_t _score = 0;

    cocos2d::Label* _labelLife;

    cocos2d::Label* _labelScore;

    uint32_t _foodNum = 0;
    const decltype(_foodNum) MAX_FOOD_NUM = 1;
};
