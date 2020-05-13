#include "Utils.h"

#include "cocos2d.h"
USING_NS_CC;

namespace visible_size {

Director* director;

Size visibleSize;
Vec2 origin;
Vec2 center;
Vec2 cleft;
Vec2 cright;
Vec2 ctop;
Vec2 cbottom;
Vec2 tleft;
Vec2 tright;
Vec2 bleft;
Vec2 bright;

float left;
float right;
float top;
float bottom;

void init() {
    director = Director::getInstance();
    visibleSize = director->getVisibleSize();
    origin = director->getVisibleOrigin();
    center = origin + visibleSize / 2;

    cleft = {origin.x, center.y};
    cright = {origin.x + visibleSize.width, center.y};
    ctop = {center.x, origin.y + visibleSize.height};
    cbottom = {center.x, origin.y};

    tleft = {origin.x, ctop.y};
    tright = {cright.x, ctop.y};
    bleft = origin;
    bright = {cright.x, origin.y};

    left = origin.x;
    right = tright.x;
    top = tleft.y;
    bottom = origin.y;
};

}
