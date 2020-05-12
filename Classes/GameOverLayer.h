#pragma once

#include "cocos2d.h"

class GameOverLayer : public cocos2d::LayerColor {
public:
    explicit GameOverLayer(uint32_t score, uint32_t goodScore);

    ~GameOverLayer() override;

private:
    cocos2d::EventListenerTouchOneByOne* _touchListener;

    const char* K_BEST_SCORE = "BEST_SCORE";
    uint32_t _bestScore;
};
