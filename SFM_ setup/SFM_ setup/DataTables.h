#pragma once
#include "ResourceIdentifiers.h"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <vector>
#include <functional>
#include <map>
#include "Actor.h"


struct Direction
{
	Direction(float angle, float distance)
		: angle(angle)
		, distance(distance)
	{
	}

	float angle;
	float distance;
};
/*
struct AircraftData
{
	int								hitpoints;
	float							speed;
	Textures::ID					texture;
	sf::IntRect						textureRect;
	sf::Time						fireInterval;
	std::vector<Direction>			directions;
	bool							hasRollAnimation;
};

struct ProjectileData
{
	int								damage;
	float							speed;
	Textures::ID					texture;
	sf::IntRect						textureRect;
};

struct PickupData
{
	std::function<void(Aircraft&)>	action;
	Textures::ID					texture;
	sf::IntRect						textureRect;
};
*/
struct ParticleData
{
	sf::Color						color;
	sf::Time						lifetime;
};
struct ActorData
{
	int		hitPoints;
	int		damageDone;
	float	speed;
	Textures::ID texture;
	std::map<Actor::State, Animation2> animations;
	std::vector<Direction>	directions;
};

/*
std::vector<AircraftData>	initializeAircraftData();
std::vector<ProjectileData>	initializeProjectileData();
std::vector<PickupData>		initializePickupData();
*/
std::map<Actor::Type, ActorData>	initializeActorData();
std::vector<ParticleData>	initializeParticleData();

