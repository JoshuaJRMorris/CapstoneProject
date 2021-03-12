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
	JsonFrameParser frames = JsonFrameParser("Media/Textures/RedBirdSolo.json");

	data[Actor::Type::RedBird].animations[Actor::State::Idle].addFrameSet(frames.getFramesFor("Red1_Fla"));
	data[Actor::Type::RedBird].animations[Actor::State::Idle].setDuration(sf::seconds(0.2f));
	data[Actor::Type::RedBird].animations[Actor::State::Idle].setRepeating(true);

	data[Actor::Type::RedBird].animations[Actor::State::Fly].addFrameSet(frames.getFramesFor("Red1_Fla"));
	data[Actor::Type::RedBird].animations[Actor::State::Fly].setDuration(sf::seconds(0.2f));
	data[Actor::Type::RedBird].animations[Actor::State::Fly].setRepeating(true);


	//data[Actor::Type::RedBird].animations[Actor::State::MoveRight].addFrameSet(frames.getFramesFor("RedMoveRight "));
	//data[Actor::Type::RedBird].animations[Actor::State::MoveRight].setDuration(sf::seconds(1.f));
	//data[Actor::Type::RedBird].animations[Actor::State::MoveRight].setRepeating(true);


	return data;
}


//std::vector<ParticleData> initializeParticleData()
//{
//	std::vector<ParticleData> data(Particle::ParticleCount);
//
//	data[Particle::Propellant].color = sf::Color(255, 255, 50);
//	data[Particle::Propellant].lifetime = sf::seconds(0.6f);
//
//	data[Particle::Smoke].color = sf::Color(50, 50, 50);
//	data[Particle::Smoke].lifetime = sf::seconds(4.f);
//
//	return data;
//}
