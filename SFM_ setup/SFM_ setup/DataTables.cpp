#include "DataTables.h"
#include "Actor.h"
#include "Projectile.h"
#include "Pickup.h"
#include "Particle.h"
#include "JsonFrameParser.h"


// For std::bind() placeholders _1, _2, ...
using namespace std::placeholders;

std::map<Actor::Type, ActorData> initializeActorData()
{

	std::map<Actor::Type, ActorData> data;
	data[Actor::Type::RedBird].texture = Textures::ID::RedBird;
	data[Actor::Type::RedBird].speed = 400;
	JsonFrameParser frames = JsonFrameParser("Media/Textures/RedBird.json");

	data[Actor::Type::RedBird].animations[Actor::State::MoveLeft].addFrameSet(frames.getFramesFor("RedMoveLeft"));
	data[Actor::Type::RedBird].animations[Actor::State::MoveLeft].setDuration(sf::seconds(1.f));
	data[Actor::Type::RedBird].animations[Actor::State::MoveLeft].setRepeating(true);

	data[Actor::Type::RedBird].animations[Actor::State::MoveRight].addFrameSet(frames.getFramesFor("RedMoveRight"));
	data[Actor::Type::RedBird].animations[Actor::State::MoveRight].setDuration(sf::seconds(1.f));
	data[Actor::Type::RedBird].animations[Actor::State::MoveRight].setRepeating(true);


	return data;
}



