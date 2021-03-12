#include "Actor.h"
#include "DataTables.h"
#include "Utility.h"
#include "Pickup.h"
#include "CommandQueue.h"
#include "SoundNode.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <cmath>
#include <random>


using namespace std::placeholders;

namespace
{
	std::mt19937 rEng{ std::random_device{}() };
	std::uniform_int_distribution<unsigned int> dist{ 40, 75 };
	const std::map<Actor::Type, ActorData> TABLE = initializeActorData();
}

Actor::Actor(Type type, const TextureHolder& textures, const FontHolder& fonts)
	: Entity(100)
	, mType(type)
	, mSprite(textures.get(TABLE.at(type).texture))
	, mTravelledDistance(0.f)
	, direction_(Direction::Up)
	, mDirectionIndex(0)
{
	for (auto a : TABLE.at(type).animations)
	{
		animations_[a.first] = a.second;
	}

	switch (type)
	{
	case Actor::Type::RedBird:
		state_ = State::Idle;
		setScale(5.f, 5.f);
		break;

	default:
		state_ = State::Idle;
		break;
	}

	mSprite.setTextureRect(sf::IntRect());
	centerOrigin(mSprite);


}

void Actor::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{

	target.draw(mSprite, states);
}

void Actor::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	//updateStates();

	auto rec = animations_.at(state_).update(dt);
	if (mType == Actor::Type::RedBird)
	{
		switch (state_)
		{
		case Actor::State::MoveUp:
			direction_ = Direction::Up;
			break;
		case Actor::State::MoveDown:
			direction_ = Direction::Down;
			break;
		case Actor::State::MoveLeft:
			direction_ = Direction::Left;
			break;
		case Actor::State::MoveRight:
			direction_ = Direction::Right;
			break;
		default:
			break;
		}
	}


	mSprite.setTextureRect(rec);
	centerOrigin(mSprite);
	if (state_ != State::Dead)
		Entity::updateCurrent(dt, commands);

	updateMovementPattern(dt);
}

unsigned int Actor::getCategory() const
{
	switch (mType)
	{
	case Type::RedBird:
		return Category::PlayerCharacter;
		break;
	}
}

sf::FloatRect Actor::getBoundingRect() const
{
	auto box = getWorldTransform().transformRect(mSprite.getGlobalBounds());
	box.width -= 39; // tighten up bounding box for more realistic collisions
	box.left += 15;
	return box;
}

bool Actor::isMarkedForRemoval() const
{
	return isDestroyed();
}

void Actor::remove()
{
	Entity::remove();
}

void Actor::setDirection(Direction tmp_Direction)
{
	direction_ = tmp_Direction;
}

Actor::Direction Actor::getDirection()
{
	return direction_;
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
float Actor::getMaxSpeed() const
{
	return TABLE.at(mType).speed;
}

void Actor::playLocalSound(CommandQueue& commands, SoundEffect::ID effect)
{
	sf::Vector2f worldPosition = getWorldPosition();

	Command command;
	command.category = Category::SoundEffect;
	command.action = derivedAction<SoundNode>(
		[effect, worldPosition](SoundNode& node, sf::Time)
		{
			node.playSound(effect, worldPosition);
		});

	commands.push(command);
}

void Actor::updateMovementPattern(sf::Time dt)
{
	// Enemy: Movement pattern
	auto directions = TABLE.at(mType).directions;
	if (!directions.empty())
	{
		if (mTravelledDistance > (directions[mDirectionIndex].distance))
		{
			mDirectionIndex = (++mDirectionIndex) % directions.size();
			mTravelledDistance = 0;
		}
		if (direction_ == Actor::Direction::Left && mType != Actor::Type::RedBird)
		{
			directions[mDirectionIndex].angle *= -1;
		}
		float radians = toRadian(directions[mDirectionIndex].angle + 90.f);
		float vx;
		float vy;
		vx = getMaxSpeed() * std::cos(radians);
		vy = getMaxSpeed() * std::sin(radians);

		setVelocity(vx, vy);
		mTravelledDistance += getMaxSpeed() * dt.asSeconds();
	}
}

void Actor::updateTexts()
{
	// Display hitpoints
	if (isDestroyed())
		mHealthDisplay->setString("");
	else
		mHealthDisplay->setString(toString(getHitpoints()) + " HP");
	mHealthDisplay->setPosition(0.f, 50.f);
	mHealthDisplay->setRotation(-getRotation());

}
