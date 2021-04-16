#pragma once
#include "Entity.h"
#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include "Animation2.h"
#include "Animation.h"
#include "TextNode.h"
#include <SFML/Graphics/Sprite.hpp>


class Actor : public Entity
{
public:
	enum  class Type
	{

		RedBird,
		BlueBird,

		TypeCount
	};
	enum class State {
		Dead, Idle, Fly, Attack
	};
	enum class Direction
	{
		Left, Right, Up, Down
	};


public:
	Actor(Type type, const TextureHolder& textures, const FontHolder& fonts);

	virtual unsigned int	getCategory() const;
	virtual sf::FloatRect	getBoundingRect() const;
	virtual void			remove();
	virtual bool 			isMarkedForRemoval() const;
	void					attack();

	void guideTowards(sf::Vector2f position);

	float					getMaxSpeed() const;

	void                                setState(State state);
	Actor::State                        getState() const;
	void                                setDirection(Direction tmp_Direction);
	Actor::Direction                    getDirection();


	void					playLocalSound(CommandQueue& commands, SoundEffect::ID effect);

private:
	void                                updateStates();
private:
	virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void 			updateCurrent(sf::Time dt, CommandQueue& commands);
	void					updateMovementPattern(sf::Time dt);


	void					updateTexts();



private:
	Type							mType;
	State							state_;
	sf::Sprite						mSprite;
	Animation						mExplosion;
	std::map<State, Animation2>		animations_;
	Direction						direction_;
	bool 					mShowExplosion;
	bool					mPlayedExplosionSound;

	sf::Vector2f			mTargetDirection;
	std::vector<Actor*>				mActiveEnemies;

	TextNode* mHealthDisplay;
	float							mTravelledDistance;
	std::size_t						mDirectionIndex;
	bool								attack_;
};