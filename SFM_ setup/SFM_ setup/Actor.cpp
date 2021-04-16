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
//#include "SceneNode.cpp"


using namespace std::placeholders;

namespace
{
	std::mt19937 rEng{ std::random_device{}() };
	std::uniform_int_distribution<unsigned int> dist{ 40, 75 };
	const std::map<Actor::Type, ActorData> TABLE = initializeActorData();
}

Actor::Actor(Type type, const TextureHolder& textures, const FontHolder& fonts)
	: Entity(TABLE.at(type).hitPoints)
	, mType(type)
	, mSprite(textures.get(TABLE.at(type).texture))
	, mExplosion(textures.get(Textures::Explosion))
	, mTravelledDistance(0.f)
	, direction_(Direction::Up)
	, mShowExplosion(true)
	, mPlayedExplosionSound(false)
	, topLeftCorner(0.f,720.f)
	, topRightCorner(1280.f,720.f)
	, bottomLeftCorner(0.f,0.f)
	, bottomRightCorner(1280.f, 720.f)
	, deadCenter(640.f, 360.f)
	, mTargetDirection()
	, mDirectionIndex(0)
	, mHealthDisplay(nullptr)
	, attack_(false)
{
	for (auto a : TABLE.at(type).animations)
	{
		animations_[a.first] = a.second;
	}

	switch (type)
	{
	case Actor::Type::RedBird:
		state_ = State::Idle;
		setScale(7.f, 7.f);
		break;
	case Actor::Type::BlueBird:
		state_ = State::Idle;
		setScale(7.f, 7.f);
		break;

	default:
		state_ = State::Idle;
		break;
	}

	std::unique_ptr<TextNode> healthDisplay(new TextNode(fonts, ""));
	mHealthDisplay = healthDisplay.get();
	attachChild(std::move(healthDisplay));

	mExplosion.setFrameSize(sf::Vector2i(256, 256));
	mExplosion.setNumFrames(16);
	mExplosion.setDuration(sf::seconds(1));

	mSprite.setTextureRect(sf::IntRect());
	centerOrigin(mSprite);
	centerOrigin(mExplosion);
	updateTexts();

}

void Actor::updateStates()
{
	if (isDestroyed())
		state_ = Actor::State::Dead;

	if (state_ == Actor::State::Attack && animations_[state_].isFinished())
		state_ = Actor::State::Fly;

	if (attack_ && state_ != Actor::State::Attack) {
		state_ = Actor::State::Attack;
		animations_[state_].restart();
		attack_ = false;
	}

	if (state_ == Actor::State::Idle && length(getVelocity()) > 0.1f)
		state_ = Actor::State::Fly;
		
}

void Actor::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{

	if (isDestroyed() && mShowExplosion)
		target.draw(mExplosion, states);
	else
		target.draw(mSprite, states);
}

void Actor::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	updateTexts();
	updateStates();

	if (isDestroyed())
	{
		
		mExplosion.update(dt);

		// Play explosion sound only once
		if (!mPlayedExplosionSound)
		{
			SoundEffect::ID soundEffect = (randomInt(2) == 0) ? SoundEffect::Explosion1 : SoundEffect::Explosion2;
			playLocalSound(commands, soundEffect);

			mPlayedExplosionSound = true;
		}
		return;
	}
	auto rec = animations_.at(state_).update(dt);

	if (state_ != State::Dead) {
		if (direction_ == Direction::Left && getVelocity().x > 0)
			direction_ = Direction::Right;
		if (direction_ == Direction::Right && getVelocity().y < 0)
			direction_ = Direction::Left;
	}

	if (direction_ == Direction::Left || direction_ == Direction::Down)
		rec = flip(rec);


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
	case Type::BlueBird:
		return Category::EnemyCharacter;
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
	return isDestroyed() && (mExplosion.isFinished() || !mShowExplosion);
}

void Actor::attack()
{
	attack_ = true;
}



void Actor::remove()
{
	Entity::remove();
	mShowExplosion = false;
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
	if (mType == Actor::Type::BlueBird) {

		switch (state_) {
		case State::Idle:
		case State::Fly:
			idleMovements(dt);		
			break;
		case State::Attack:
			//initate attack sequence
			break;
		case State::BeenAttacked:
			//initiate out of idle
			break;
		case State::RunAway:
			//initiate findNearestCorner

			break;
		}
	}
}

void Actor::idleMovements(sf::Time dt)
{
	auto directions = TABLE.at(mType).directions;
	if (!directions.empty()) {
		if (mTravelledDistance > (directions[mDirectionIndex].distance))
		{
			direction_ = Direction::Left;
			mDirectionIndex = (++mDirectionIndex) % directions.size();
			mTravelledDistance = 0;
			direction_ = Direction::Right;
		}

		float radians = toRadian(directions[mDirectionIndex].angle + 90.f);
		float vx = getMaxSpeed() * std::cos(radians);
		float vy = getMaxSpeed() * std::sin(radians);


		setVelocity(vx, vy);
		mTravelledDistance += getMaxSpeed() * dt.asSeconds();
	}
}

sf::Vector2f Actor::findNearestCorner()
{
	sf::Vector2f currentLocation = this->getPosition();
	// quadrant 1
	if (currentLocation.x < deadCenter.x && currentLocation.y > deadCenter.y) {
		return topLeftCorner;
	}
	//quadrant 2
	else if (currentLocation.x > deadCenter.x && currentLocation.y > deadCenter.y) {
		return topRightCorner;
	}
	//quadrant 3
	else if (currentLocation.x < deadCenter.x && currentLocation.y < deadCenter.y) {
		return bottomLeftCorner;
	}
	//quadrant 4
	else if (currentLocation.x > deadCenter.x && currentLocation.y < deadCenter.y) {
		return bottomRightCorner;
	}
	else {
		return deadCenter;
	}
}


void Actor::guideTowards(sf::Vector2f position)
{
	mTargetDirection = unitVector(position - getWorldPosition());
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
