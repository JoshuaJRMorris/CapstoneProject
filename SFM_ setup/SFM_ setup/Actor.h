#pragma once

#include "Entity.h"
#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include "Animation2.h"
#include "TextNode.h"

#include <SFML/Graphics/Sprite.hpp>

class Actor : public Entity
{
public:
	enum class Type {
		Hero2, Zombie1, Zombie2, Zombie3, Zombie4, Zombie5
	};

	enum class State {
		Attack, Dead, Idle, Rise, Walk, Run, Jump, count
	};

	enum class Direction {
		Left, Right, Up, Down
	};

public:
	Actor(Type type, const TextureHolder& textures, const FontHolder& fonts);
	~Actor() = default;

	unsigned int			getCategory() const override;
	sf::FloatRect			getBoundingRect() const override;
	float					getMaxSpeed() const;

	bool					isMarkedForRemoval() const override;

	void					attack();
	void					setState(State state);
	Actor::State getState() const;
	int						attackPoints() const;



private:
	void					updateStates();
	void					updateCurrent(sf::Time dt, CommandQueue& commands) override;
	void					drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
	void					updateMovementPattern(sf::Time dt);
	void					updateTexts();

private:
	Type								type_;
	State								state_;
	mutable sf::Sprite					sprite_;
	std::map<State, Animation2>			animations_;
	Direction							direction_;
	TextNode*							healthDisplay_;

	float								travelDistance_;
	std::size_t							directionIndex_;
	bool								attack_;

};

