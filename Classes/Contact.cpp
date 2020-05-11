#include "Contact.h"
#include "Utils.h"

Contact::Contact(float wallWidth) : _wallWidth(wallWidth + 4) {
    autorelease();
    scheduleUpdate();
}

Contact::~Contact() {
    unscheduleUpdate();
}

void Contact::addBullet(Bullet* bullet) {
    _bullets.emplace_back(bullet);
}

void Contact::addTank(Tank* tank) {
    _tanks.emplace_back(tank);
}

void Contact::update(float delta) {
    Node::update(delta);

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

            auto maxDistance = tk->getContentSize().width + bullet->getContentSize().width;
            maxDistance /= 2;
            auto distance = tk->getPosition().getDistance(bullet->getPosition());
            if (distance < maxDistance) {
                bullet->HasBoom = true;
                tk->harm(bullet);
            }
        }
    };
    for (auto it = _bullets.cbegin(); it != _bullets.cend();) {
        bulletLogic(*it);

        if ((*it)->shouldDisappear()) {
            (*it)->removeFromParent();
            _bullets.erase(it);
        }
        else {
            it++;
        }
    }

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
    for (auto it = _tanks.cbegin(); it != _tanks.cend();) {
        tankLogic(*it);

        if ((*it)->isDie()) {
            _tanks.erase(it);
        }
        else {
            it++;
        }
    }
}
