#include "Contact.h"
#include "Utils.h"

USING_NS_CC;

Contact::Contact(float wallWidth) : _wallWidth(wallWidth + 4) {
    autorelease();
    scheduleUpdate();
}

Contact::~Contact() {
    unscheduleUpdate();
    _userTank->release();
}

void Contact::addBullet(Bullet* bullet) {
    _bullets.pushBack(bullet);
}

void Contact::addTank(Tank* tank) {
    _tanks.pushBack(tank);
}

void Contact::setUserTank(Tank* tank) {
    _userTank = tank;
    tank->retain();
}

void Contact::addFood(Food* food) {
    _foods.pushBack(food);
}

void Contact::update(float delta) {
    Node::update(delta);
    updateBullet(delta);
    updateTank(delta);
    updateFood(delta);
}

void Contact::updateBullet(float delta) {
    auto bulletLogic = [&](Bullet* bullet) {
        auto visibleSize = Director::getInstance()->getVisibleSize();
        auto origin = Director::getInstance()->getVisibleOrigin();

        // 墙内外
        auto pos = bullet->getPosition();
        auto size = bullet->getContentSize();

        /// 撞墙反弹
        auto rotation = bullet->getRotation();
        bool bounce = false;

        // 更正范围
        for(;;) {
            if (rotation < 0) {
                rotation += 360;
                continue;
            } else if (rotation > 360) {
                rotation -= 360;
                continue;
            }
            break;
        }

        // 左
        if (pos.x < origin.x + _wallWidth) {
            bounce = true;
            if (90 <= rotation and rotation <= 180) {
                rotation = 90 - (rotation - 90);
            } else if (180 <= rotation and rotation <= 270) {
                rotation = -(rotation - 180);
            }
        }
        // 右
        else if (pos.x > origin.x + visibleSize.width - _wallWidth) {
            bounce = true;
            if (0 <= rotation and rotation <= 90) {
                rotation = 90 + (90 - rotation);
            }
            else if (270 <= rotation and rotation <= 360) {
                rotation = 270 - (rotation - 270);
            }
        }
        // 下
        else if (pos.y < origin.y + _wallWidth) {
            bounce = true;
            if (90 <= rotation and rotation <= 180) {
                rotation = 180 + (180 - rotation);
            }
            else if (0 <= rotation and rotation <= 90) {
                rotation = -rotation;
            }
        }
        // 上
        else if (pos.y > origin.y + visibleSize.height - _wallWidth) {
            bounce = true;
            if (180 <= rotation and rotation <= 270) {
                rotation = 180 - (rotation - 180);
            }
            else if (270 <= rotation and rotation <= 360) {
                rotation = 360 - rotation;
            }
        }
        if (bounce) {
            bullet->setRotation(rotation);
            bullet->BounceCount++;
        }

        // 碰到任意坦克
        for(const auto& tk : _tanks) {
            // 忽略短时间内自己发出的子弹
            if ((tk == bullet->FromTank)
            && (::utils::nowMs() - bullet->FromTime < 1000)
            ) {
                continue;
            }

            auto maxDistance = tk->getContentSize().width * tk->getScale()
                    + bullet->getContentSize().width * bullet->getScale();
            maxDistance /= 2;
            auto distance = tk->getPosition().getDistance(bullet->getPosition());
            if (distance < maxDistance) {
                tk->harm(bullet);
                bullet->boom();
            }
        }
    };

    for (auto it = _bullets.begin(); it != _bullets.end();) {
        auto bullet = *it;
        bulletLogic(bullet);

        if (bullet->hasBoom()) {
            _bullets.erase(it);
        }
        else {
            it++;
        }
    }
}

void Contact::updateTank(float delta) {
    auto tankLogic = [&](Tank* tank) {
        auto visibleSize = Director::getInstance()->getVisibleSize();
        auto origin = Director::getInstance()->getVisibleOrigin();

        // 墙内外
        auto pos = tank->getPosition();
        auto tankSize = tank->getContentSize() * tank->getScale();
        ::utils::setLimit(pos.x
                , origin.x + tankSize.width / 2 + _wallWidth
                , origin.x + visibleSize.width - tankSize.width / 2 - _wallWidth
        );
        ::utils::setLimit(pos.y
                , origin.y + tankSize.height / 2 + _wallWidth
                , origin.y + visibleSize.height - tankSize.width / 2 - _wallWidth
        );
        tank->setPosition(pos);
    };
    for (auto it = _tanks.begin(); it != _tanks.end();) {
        auto tk = *it;
        tankLogic(tk);
        if (tk->isDie()) {
            _tanks.erase(it);
        }
        else {
            it++;
        }
    }
}

void Contact::updateFood(float delta) {
    auto logic = [&](Food* food) {
        auto visibleSize = Director::getInstance()->getVisibleSize();
        auto origin = Director::getInstance()->getVisibleOrigin();

        // 墙内外
        auto pos = food->getPosition();
        auto size = food->getContentSize();

        // 碰到玩家坦克
        const auto& tk = _userTank;
        auto maxDistance = tk->getContentSize().width + food->getContentSize().width;
        maxDistance /= 3;
        auto distance = tk->getPosition().getDistance(food->getPosition());
        bool isBound = distance < maxDistance;
        if (isBound) {
            tk->addLife(food->Energy);
            food->eatBy(tk);
        }
    };
    for (auto it = _foods.begin(); it != _foods.end();) {
        auto food = *it;
        logic(food);
        if (food->hasEat()) {
            _foods.erase(it);
        }
        else {
            it++;
        }
    }
}
