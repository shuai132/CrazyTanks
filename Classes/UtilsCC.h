#pragma once

#include "cocos2d.h"

namespace utils_cc {

USING_NS_CC;

extern Director* director;

extern Size visibleSize;
extern Vec2 origin;
extern Vec2 center;
extern Vec2 cleft;
extern Vec2 cright;
extern Vec2 ctop;
extern Vec2 cbottom;
extern Vec2 tleft;
extern Vec2 tright;
extern Vec2 bleft;
extern Vec2 bright;

extern float left;
extern float right;
extern float top;
extern float bottom;

void init();

using RefKeeper = cocos2d::Vector<Ref*>;

}
