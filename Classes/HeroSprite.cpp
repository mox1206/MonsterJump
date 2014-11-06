#include "HeroSprite.h"
#include "VisibleRect.h"
#include "Utils.h"

HeroSprite::HeroSprite()
{
}


HeroSprite::~HeroSprite()
{
}

bool HeroSprite::initEntity()
{
	if (!Sprite::initWithFile(m_sSource)) return false;
	getTexture()->setAliasTexParameters();
	this->setPosition(this->_initVec + Point(this->getContentSize().width / 2, 0));
    setEntityType(EntityType::kType_Hero);
    this->setAnchorPoint(Point(0.5, 0));
	_offset = 0.0f;
	_moveUpOrDown = MoveDir_None;
	_moveLeftOrRight = MoveDir_None;
	return true;
}

void HeroSprite::setAccelerometerEnabled(bool enabled)
{
	if (enabled != _accelerometerEnabled)
	{
		_accelerometerEnabled = enabled;

		Device::setAccelerometerEnabled(enabled);
		Device::setAccelerometerInterval(0.1f);

		_eventDispatcher->removeEventListener(_accelerationListener);
		_accelerationListener = nullptr;

		if (enabled)
		{
			_accelerationListener = EventListenerAcceleration::create(CC_CALLBACK_2(HeroSprite::onAcceleration, this));
			_eventDispatcher->addEventListenerWithSceneGraphPriority(_accelerationListener, this);
		}
	}
}

void HeroSprite::onAcceleration(Acceleration* acc, Event* unused_event)
{
    if( unused_event->getType() == Event::Type::ACCELERATION)
    {
        float sensitivity = 25.0;
        float maxVelocity = 30.0f;
        _offset = acc->x * sensitivity;
        if (_offset >= maxVelocity)
        {
            _offset = maxVelocity;
        }
        else if (_offset <= -maxVelocity)
        {
            _offset = -maxVelocity;
        }
		this->changeDir(_offset > 0 ? MoveDir_Left: MoveDir_Right);
    }
}

void HeroSprite::startJump()
{
	_previousPos = getPosition();
	this->setPositionY(getPositionY());
	this->setAccelerometerEnabled(true);
	this->scheduleUpdate();
}

void HeroSprite::onExit()
{
	BaseEntity::onExit();
	this->stopAllActions();
	this->setAccelerometerEnabled(false);
	this->unscheduleUpdate();
}

void HeroSprite::setPositionY(float y)
{
	this->_moveUpOrDown = MoveDir_None;
	this->_time = 0.0f;
	BaseEntity::setPositionY(y);
	this->_startPos = y;
	this->changeState(kState_jump);
}

void HeroSprite::update(float dt)
{
	_time += dt * 3.0;
	float s = VELOCITY * _time + ACCELERATED * _time * _time / 2;
	MoveDir dir = _previousPos.y > getPositionY() ? MoveDir_Down : MoveDir_Up;
	this->changeDir(dir);
	_previousPos = getPosition();
	if (getPositionX() > VisibleRect::right().x + getContentSize().width / 2)
	{
		setPositionX(-getContentSize().width / 2);
	}
	else if (getPositionX() < -getContentSize().width / 2)
	{
		setPositionX(VisibleRect::right().x + getContentSize().width / 2);
	}
	setPosition(Point(getPositionX() + _offset, this->_startPos + s));
}

void HeroSprite::changeState(HeroState state)
{
	this->stopAllActions();
	Animate* animate = nullptr;
	switch (state)
	{
	case kState_fall:
		animate = Utils::getAnimate("fall/rabbit_fall_", 1, 8);
		break;
	case kState_jump:
		animate = Utils::getAnimate("jump/rabbit_jump_", 1, 7);
		break;
	case kState_rush:
		animate = Utils::getAnimate("rush/rabbit_rush_", 1, 8);
		break;
	case kState_rush_end:
		animate = Utils::getAnimate("rush_end/rabbit_rush2_", 1, 8);
		break;
	case kState_walk:
		animate = Utils::getAnimate("walk/rabbit_walk_", 1, 14);
		break;
	}
	this->runAction(animate);
}

void HeroSprite::changeDir(MoveDir dir)
{
	if ((dir == MoveDir_Left || dir == MoveDir_Right) && _moveLeftOrRight != dir)
	{
		_moveLeftOrRight = dir;
	}
	else if ((dir == MoveDir_Down || dir == MoveDir_Up) && _moveUpOrDown != dir)
	{
		_moveUpOrDown = dir;
	}
}