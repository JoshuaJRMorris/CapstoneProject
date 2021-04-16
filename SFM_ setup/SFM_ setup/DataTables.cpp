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
	JsonFrameParser frames = JsonFrameParser("Media/Textures/Birds.json");

	data[Actor::Type::RedBird].texture = Textures::ID::RedBird;
	data[Actor::Type::RedBird].speed = 400;
	//data[Actor::Type::RedBird].damageDone = 10;
	data[Actor::Type::RedBird].hitPoints = 100;

	data[Actor::Type::RedBird].animations[Actor::State::Idle].addFrameSet(frames.getFramesFor("RedFlap"));
	data[Actor::Type::RedBird].animations[Actor::State::Idle].setDuration(sf::seconds(0.2f));
	data[Actor::Type::RedBird].animations[Actor::State::Idle].setRepeating(true);

	data[Actor::Type::RedBird].animations[Actor::State::Fly].addFrameSet(frames.getFramesFor("RedFlap"));
	data[Actor::Type::RedBird].animations[Actor::State::Fly].setDuration(sf::seconds(0.2f));
	data[Actor::Type::RedBird].animations[Actor::State::Fly].setRepeating(true);

	data[Actor::Type::RedBird].animations[Actor::State::Attack].addFrameSet(frames.getFramesFor("RedBirdAttack1"));
	data[Actor::Type::RedBird].animations[Actor::State::Attack].setDuration(sf::seconds(0.5f));
	data[Actor::Type::RedBird].animations[Actor::State::Attack].setRepeating(false);

	data[Actor::Type::RedBird].animations[Actor::State::Dead].addFrameSet(frames.getFramesFor("RedFlap"));
	data[Actor::Type::RedBird].animations[Actor::State::Dead].setDuration(sf::seconds(1.f));
	data[Actor::Type::RedBird].animations[Actor::State::Dead].setRepeating(false);


	data[Actor::Type::BlueBird].texture = Textures::ID::BlueBird;
	data[Actor::Type::BlueBird].speed = 40;
	data[Actor::Type::BlueBird].hitPoints = 200;
	data[Actor::Type::BlueBird].directions.emplace_back(Direction(45.f, 50.f));
	data[Actor::Type::BlueBird].directions.emplace_back(Direction(135.f, 50.f));


	data[Actor::Type::BlueBird].animations[Actor::State::Idle].addFrameSet(frames.getFramesFor("Blue"));
	data[Actor::Type::BlueBird].animations[Actor::State::Idle].setDuration(sf::seconds(0.2f));
	data[Actor::Type::BlueBird].animations[Actor::State::Idle].setRepeating(true);

	data[Actor::Type::BlueBird].animations[Actor::State::Fly].addFrameSet(frames.getFramesFor("Blue"));
	data[Actor::Type::BlueBird].animations[Actor::State::Fly].setDuration(sf::seconds(0.2f));
	data[Actor::Type::BlueBird].animations[Actor::State::Fly].setRepeating(true);

	data[Actor::Type::BlueBird].animations[Actor::State::Dead].addFrameSet(frames.getFramesFor("Explosion"));
	data[Actor::Type::BlueBird].animations[Actor::State::Dead].setDuration(sf::seconds(1.f));
	data[Actor::Type::BlueBird].animations[Actor::State::Dead].setRepeating(false);



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
