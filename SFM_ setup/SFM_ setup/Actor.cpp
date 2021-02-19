#include "Actor.h"
#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include "jsonFrameParser.h"
#include "Utility.h"
#include "DataTables.h"
#include "TextNode.h"

#include <SFML/Graphics/RenderTarget.hpp>

namespace 
{
	const std::map<Actor::Type, ActorData> TABLE = InitializeActorData();
}

Actor::Actor(Type type, const TextureHolder& textures, const FontHolder& fonts)
	: Entity(100)
	, type_(type)
	, state_(State::Walk)
	, sprite_(textures.get(TABLE.at(type).texture))
	, direction_(Direction::Right)
	, travelDistance_(0.f)
	, directionIndex_(0)
	, attack_(false)
{
	for (auto a : TABLE.at(type).animations)
	{
		animations_[a.first] = a.second;
	}

	if (Actor::getCategory() == Category::Zombie)
		state_ = State::Rise;

	sprite_.setTextureRect(sf::IntRect());
	centerOrigin(sprite_);

	std::unique_ptr<TextNode> health(new TextNode(fonts, ""));
	healthDisplay_ = health.get();
	attachChild(std::move(health));

	updateTexts();
}

unsigned int Actor::getCategory() const
{
	switch (type_)
	{
	case Type::Hero2:
		return Category::Hero;
		break;
	case Type::Zombie1:
	case Type::Zombie2:
	case Type::Zombie3:
	case Type::Zombie4:
	case Type::Zombie5:
		return Category::Zombie;
		break;
	}

	return Category::Zombie;
}

void Actor::updateMovementPattern(sf::Time dt)
{
	auto directions = TABLE.at(type_).directions;

	if (!directions.empty()) {
		if (travelDistance_ > (directions[directionIndex_].distance))
		{
			directionIndex_ = (++directionIndex_) % directions.size();
			travelDistance_ = 0;
		}

		float radians = toRadian(directions[directionIndex_].angle + 90.f);
		float vx = getMaxSpeed() * std::cos(radians);
		float vy = getMaxSpeed() * std::sin(radians);

		setVelocity(vx, vy);
		travelDistance_ += getMaxSpeed() * dt.asSeconds();
	}
}


sf::FloatRect Actor::getBoundingRect() const
{
	auto box = getWorldTransform().transformRect(sprite_.getGlobalBounds());
	box.width -= 30;
	box.left += 15;
	return box;
}

float Actor::getMaxSpeed() const
{
	return TABLE.at(type_).speed;
}

bool Actor::isMarkedForRemoval() const
{
	return false;
}

void Actor::attack()
{
	attack_ = true;
}

void Actor::setState(State state)
{
	state_ = state;
	animations_[state_].restart();
}

Actor::State Actor::getState() const
{
	return state_;
}

int Actor::attackPoints() const
{
	if (type_ == Type::Hero2 && state_ == Actor::State::Attack)
		return TABLE.at(type_).damageDone;
	else if (type_ != Type::Hero2)
		return TABLE.at(type_).damageDone;
	else
		return 0;
}

void Actor::updateStates()
{
	if (isDestroyed())
		state_ = Actor::State::Dead;

	if (state_ == Actor::State::Attack && animations_[state_].isFinished())
		state_ = Actor::State::Walk;

	if (attack_ && state_ != Actor::State::Attack) {
		state_ = Actor::State::Attack;
		animations_[state_].restart();
		attack_ = false;
	}

	if (state_ == Actor::State::Rise && animations_[state_].isFinished())
		state_ = Actor::State::Idle;
	if (state_ == Actor::State::Idle && length(getVelocity()) > 0.1f)
		state_ = Actor::State::Walk;
}

void Actor::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	updateStates();

	auto rec = animations_.at(state_).update(dt);

	if (state_ != State::Dead) {
		if (direction_ == Direction::Left && getVelocity().x > 0)
			direction_ = Direction::Right;
		if (direction_ == Direction::Right && getVelocity().y < 0)
			direction_ = Direction::Left;
	}

	if (direction_ == Direction::Left)
		rec = flip(rec);

	sprite_.setTextureRect(rec);
	centerOrigin(sprite_);

	if (state_ != State::Dead)
		Entity::updateCurrent(dt, commands);

	updateMovementPattern(dt);
	updateTexts();
}

void Actor::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite_, states);
}



void Actor::updateTexts()
{
	healthDisplay_->setString(std::to_string(getHitpoints()) + "HP");
	healthDisplay_->setPosition(0.f, 70.f);
	healthDisplay_->setRotation(-getRotation());
}
