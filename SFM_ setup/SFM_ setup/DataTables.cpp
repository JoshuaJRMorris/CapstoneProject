 #include "DataTables.h"
#include "Aircraft.h"
#include "Projectile.h"
#include "Pickup.h"
#include "Particle.h"
#include "jsonFrameParser.h"


// For std::bind() placeholders _1, _2, ...
using namespace std::placeholders;

//std::vector<AircraftData> initializeAircraftData()
//{
//	std::vector<AircraftData> data(Aircraft::TypeCount);
//
//	data[Aircraft::Eagle].hitpoints = 100;
//	data[Aircraft::Eagle].speed = 200.f;
//	data[Aircraft::Eagle].fireInterval = sf::seconds(1);
//	data[Aircraft::Eagle].texture = Textures::Entities;
//	data[Aircraft::Eagle].textureRect = sf::IntRect(0, 0, 48, 64);
//	data[Aircraft::Eagle].hasRollAnimation = true;
//
//	data[Aircraft::Raptor].hitpoints = 20;
//	data[Aircraft::Raptor].speed = 80.f;
//	data[Aircraft::Raptor].texture = Textures::Entities;
//	data[Aircraft::Raptor].textureRect = sf::IntRect(144, 0, 84, 64);
//	data[Aircraft::Raptor].directions.push_back(Direction(+45.f, 80.f));
//	data[Aircraft::Raptor].directions.push_back(Direction(-45.f, 160.f));
//	data[Aircraft::Raptor].directions.push_back(Direction(+45.f, 80.f));
//	data[Aircraft::Raptor].fireInterval = sf::Time::Zero;
//	data[Aircraft::Raptor].hasRollAnimation = false;
//
//	data[Aircraft::Avenger].hitpoints = 40;
//	data[Aircraft::Avenger].speed = 50.f;
//	data[Aircraft::Avenger].texture = Textures::Entities;
//	data[Aircraft::Avenger].textureRect = sf::IntRect(228, 0, 60, 59);
//	data[Aircraft::Avenger].directions.push_back(Direction(+45.f, 50.f));
//	data[Aircraft::Avenger].directions.push_back(Direction(0.f, 50.f));
//	data[Aircraft::Avenger].directions.push_back(Direction(-45.f, 100.f));
//	data[Aircraft::Avenger].directions.push_back(Direction(0.f, 50.f));
//	data[Aircraft::Avenger].directions.push_back(Direction(+45.f, 50.f));
//	data[Aircraft::Avenger].fireInterval = sf::seconds(2);
//	data[Aircraft::Avenger].hasRollAnimation = false;
//
//	return data;
//}

//std::vector<ProjectileData> initializeProjectileData()
//{
//	std::vector<ProjectileData> data(Projectile::TypeCount);
//
//	data[Projectile::AlliedBullet].damage = 10;
//	data[Projectile::AlliedBullet].speed = 300.f;
//	data[Projectile::AlliedBullet].texture = Textures::Entities;
//	data[Projectile::AlliedBullet].textureRect = sf::IntRect(175, 64, 3, 14);
//
//	data[Projectile::EnemyBullet].damage = 10;
//	data[Projectile::EnemyBullet].speed = 300.f;
//	data[Projectile::EnemyBullet].texture = Textures::Entities;
//	data[Projectile::EnemyBullet].textureRect = sf::IntRect(178, 64, 3, 14);
//
//	data[Projectile::Missile].damage = 200;
//	data[Projectile::Missile].speed = 150.f;
//	data[Projectile::Missile].texture = Textures::Entities;
//	data[Projectile::Missile].textureRect = sf::IntRect(160, 64, 15, 32);
//
//	return data;
//}

//std::vector<PickupData> initializePickupData()
//{
//	std::vector<PickupData> data(Pickup::TypeCount);
//
//	data[Pickup::HealthRefill].texture = Textures::Entities;
//	data[Pickup::HealthRefill].textureRect = sf::IntRect(0, 64, 40, 40);
//	data[Pickup::HealthRefill].action = [](Aircraft& a) { a.repair(25); };
//
//	data[Pickup::MissileRefill].texture = Textures::Entities;
//	data[Pickup::MissileRefill].textureRect = sf::IntRect(40, 64, 40, 40);
//	data[Pickup::MissileRefill].action = std::bind(&Aircraft::collectMissiles, _1, 3);
//
//	data[Pickup::FireSpread].texture = Textures::Entities;
//	data[Pickup::FireSpread].textureRect = sf::IntRect(80, 64, 40, 40);
//	data[Pickup::FireSpread].action = std::bind(&Aircraft::increaseSpread, _1);
//
//	data[Pickup::FireRate].texture = Textures::Entities;
//	data[Pickup::FireRate].textureRect = sf::IntRect(120, 64, 40, 40);
//	data[Pickup::FireRate].action = std::bind(&Aircraft::increaseFireRate, _1);
//
//	return data;
//}

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

std::map<Actor::Type, ActorData> InitializeActorData()
{
	std::map<Actor::Type, ActorData> data;

	//Player character - lumberjack
	//assign texture
	data[Actor::Type::Hero2].texture = Textures::ID::Hero2;
	//assign hit points
	data[Actor::Type::Hero2].hitPoints = 100;
	//assign damage power
	data[Actor::Type::Hero2].damageDone = 5;
	//assign speed
	data[Actor::Type::Hero2].speed = 50;

	//get the sprite sheet through the json frame parser
	JsonFrameParser frames = JsonFrameParser("Media/Textures/hero2.json");

	//assign the frames for the animation of the respective name, in this case, idle
	data[Actor::Type::Hero2].animations[Actor::State::Idle].addFrameSet(frames.getFramesFor("idle"));
	//set the duration per frams
	data[Actor::Type::Hero2].animations[Actor::State::Idle].setDuration(sf::seconds(1.f));
	//chose to repeat or not
	data[Actor::Type::Hero2].animations[Actor::State::Idle].setRepeating(true);

	data[Actor::Type::Hero2].animations[Actor::State::Jump].addFrameSet(frames.getFramesFor("jump"));
	data[Actor::Type::Hero2].animations[Actor::State::Jump].setDuration(sf::seconds(1.f));
	data[Actor::Type::Hero2].animations[Actor::State::Jump].setRepeating(true);

	data[Actor::Type::Hero2].animations[Actor::State::Attack].addFrameSet(frames.getFramesFor("attack"));
	data[Actor::Type::Hero2].animations[Actor::State::Attack].setDuration(sf::seconds(1.f));
	data[Actor::Type::Hero2].animations[Actor::State::Attack].setRepeating(false);

	data[Actor::Type::Hero2].animations[Actor::State::Walk].addFrameSet(frames.getFramesFor("walk"));
	data[Actor::Type::Hero2].animations[Actor::State::Walk].setDuration(sf::seconds(1.f));
	data[Actor::Type::Hero2].animations[Actor::State::Walk].setRepeating(true);

	data[Actor::Type::Hero2].animations[Actor::State::Run].addFrameSet(frames.getFramesFor("run"));
	data[Actor::Type::Hero2].animations[Actor::State::Run].setDuration(sf::seconds(1.f));
	data[Actor::Type::Hero2].animations[Actor::State::Run].setRepeating(true);

	data[Actor::Type::Hero2].animations[Actor::State::Dead].addFrameSet(frames.getFramesFor("dead"));
	data[Actor::Type::Hero2].animations[Actor::State::Dead].setDuration(sf::seconds(1.f));
	data[Actor::Type::Hero2].animations[Actor::State::Dead].setRepeating(false);


	//Zombie 1
	data[Actor::Type::Zombie1].texture = Textures::ID::Zombie1;
	data[Actor::Type::Zombie1].hitPoints = 100;
	data[Actor::Type::Zombie1].damageDone = 1;
	data[Actor::Type::Zombie1].speed = 50;

	frames = JsonFrameParser("Media/Textures/zombie1.json");

	data[Actor::Type::Zombie1].animations[Actor::State::Idle].addFrameSet(frames.getFramesFor("idle"));
	data[Actor::Type::Zombie1].animations[Actor::State::Idle].setDuration(sf::seconds(1.f));
	data[Actor::Type::Zombie1].animations[Actor::State::Idle].setRepeating(true);
	data[Actor::Type::Zombie1].animations[Actor::State::Attack].addFrameSet(frames.getFramesFor("attack"));
	data[Actor::Type::Zombie1].animations[Actor::State::Attack].setDuration(sf::seconds(1.f));
	data[Actor::Type::Zombie1].animations[Actor::State::Attack].setRepeating(true);
	data[Actor::Type::Zombie1].animations[Actor::State::Walk].addFrameSet(frames.getFramesFor("walk"));
	data[Actor::Type::Zombie1].animations[Actor::State::Walk].setDuration(sf::seconds(1.f));
	data[Actor::Type::Zombie1].animations[Actor::State::Walk].setRepeating(true);
	data[Actor::Type::Zombie1].animations[Actor::State::Dead].addFrameSet(frames.getFramesFor("dead"));
	data[Actor::Type::Zombie1].animations[Actor::State::Dead].setDuration(sf::seconds(1.f));
	data[Actor::Type::Zombie1].animations[Actor::State::Dead].setRepeating(false);
	data[Actor::Type::Zombie1].animations[Actor::State::Rise].addFrameSet(frames.getFramesFor("rise"));
	data[Actor::Type::Zombie1].animations[Actor::State::Rise].setDuration(sf::seconds(2.f));
	data[Actor::Type::Zombie1].animations[Actor::State::Rise].setRepeating(false);

	data[Actor::Type::Zombie1].directions.emplace_back(Direction(45.f, 50.f));
	data[Actor::Type::Zombie1].directions.emplace_back(Direction(-45.f, 100.f));
	data[Actor::Type::Zombie1].directions.emplace_back(Direction(45.f, 50.f));


	//zombie 2
	data[Actor::Type::Zombie2].texture = Textures::ID::Zombie2;
	data[Actor::Type::Zombie2].hitPoints = 100;
	data[Actor::Type::Zombie2].damageDone = 1;
	data[Actor::Type::Zombie2].speed = 50;

	frames = JsonFrameParser("Media/Textures/zombie2.json");

	data[Actor::Type::Zombie2].animations[Actor::State::Idle].addFrameSet(frames.getFramesFor("idle"));
	data[Actor::Type::Zombie2].animations[Actor::State::Idle].setDuration(sf::seconds(1.f));
	data[Actor::Type::Zombie2].animations[Actor::State::Idle].setRepeating(true);
	data[Actor::Type::Zombie2].animations[Actor::State::Attack].addFrameSet(frames.getFramesFor("attack"));
	data[Actor::Type::Zombie2].animations[Actor::State::Attack].setDuration(sf::seconds(1.f));
	data[Actor::Type::Zombie2].animations[Actor::State::Attack].setRepeating(true);
	data[Actor::Type::Zombie2].animations[Actor::State::Walk].addFrameSet(frames.getFramesFor("walk"));
	data[Actor::Type::Zombie2].animations[Actor::State::Walk].setDuration(sf::seconds(1.f));
	data[Actor::Type::Zombie2].animations[Actor::State::Walk].setRepeating(true);
	data[Actor::Type::Zombie2].animations[Actor::State::Dead].addFrameSet(frames.getFramesFor("dead"));
	data[Actor::Type::Zombie2].animations[Actor::State::Dead].setDuration(sf::seconds(1.f));
	data[Actor::Type::Zombie2].animations[Actor::State::Dead].setRepeating(false);
	data[Actor::Type::Zombie2].animations[Actor::State::Rise].addFrameSet(frames.getFramesFor("rise"));
	data[Actor::Type::Zombie2].animations[Actor::State::Rise].setDuration(sf::seconds(2.f));
	data[Actor::Type::Zombie2].animations[Actor::State::Rise].setRepeating(false);

	data[Actor::Type::Zombie2].directions.emplace_back(Direction(45.f, 50.f));
	data[Actor::Type::Zombie2].directions.emplace_back(Direction(-45.f, 100.f));
	data[Actor::Type::Zombie2].directions.emplace_back(Direction(45.f, 50.f));


	//zombie 3
	data[Actor::Type::Zombie3].texture = Textures::ID::Zombie3;
	data[Actor::Type::Zombie3].hitPoints = 100;
	data[Actor::Type::Zombie3].damageDone = 1;
	data[Actor::Type::Zombie3].speed = 50;

	frames = JsonFrameParser("Media/Textures/zombie3.json");

	data[Actor::Type::Zombie3].animations[Actor::State::Idle].addFrameSet(frames.getFramesFor("idle"));
	data[Actor::Type::Zombie3].animations[Actor::State::Idle].setDuration(sf::seconds(1.f));
	data[Actor::Type::Zombie3].animations[Actor::State::Idle].setRepeating(true);
	data[Actor::Type::Zombie3].animations[Actor::State::Attack].addFrameSet(frames.getFramesFor("attack"));
	data[Actor::Type::Zombie3].animations[Actor::State::Attack].setDuration(sf::seconds(1.f));
	data[Actor::Type::Zombie3].animations[Actor::State::Attack].setRepeating(true);
	data[Actor::Type::Zombie3].animations[Actor::State::Walk].addFrameSet(frames.getFramesFor("walk"));
	data[Actor::Type::Zombie3].animations[Actor::State::Walk].setDuration(sf::seconds(1.f));
	data[Actor::Type::Zombie3].animations[Actor::State::Walk].setRepeating(true);
	data[Actor::Type::Zombie3].animations[Actor::State::Dead].addFrameSet(frames.getFramesFor("dead"));
	data[Actor::Type::Zombie3].animations[Actor::State::Dead].setDuration(sf::seconds(1.f));
	data[Actor::Type::Zombie3].animations[Actor::State::Dead].setRepeating(false);
	data[Actor::Type::Zombie3].animations[Actor::State::Rise].addFrameSet(frames.getFramesFor("rise"));
	data[Actor::Type::Zombie3].animations[Actor::State::Rise].setDuration(sf::seconds(2.f));
	data[Actor::Type::Zombie3].animations[Actor::State::Rise].setRepeating(false);

	data[Actor::Type::Zombie3].directions.emplace_back(Direction(45.f, 50.f));
	data[Actor::Type::Zombie3].directions.emplace_back(Direction(-45.f, 100.f));
	data[Actor::Type::Zombie3].directions.emplace_back(Direction(45.f, 50.f));


	//zombie 4
	data[Actor::Type::Zombie4].texture = Textures::ID::Zombie4;
	data[Actor::Type::Zombie4].hitPoints = 100;
	data[Actor::Type::Zombie4].damageDone = 1;
	data[Actor::Type::Zombie4].speed = 50;

	frames = JsonFrameParser("Media/Textures/zombie4.json");

	data[Actor::Type::Zombie4].animations[Actor::State::Idle].addFrameSet(frames.getFramesFor("idle"));
	data[Actor::Type::Zombie4].animations[Actor::State::Idle].setDuration(sf::seconds(1.f));
	data[Actor::Type::Zombie4].animations[Actor::State::Idle].setRepeating(true);
	data[Actor::Type::Zombie4].animations[Actor::State::Attack].addFrameSet(frames.getFramesFor("attack"));
	data[Actor::Type::Zombie4].animations[Actor::State::Attack].setDuration(sf::seconds(1.f));
	data[Actor::Type::Zombie4].animations[Actor::State::Attack].setRepeating(true);
	data[Actor::Type::Zombie4].animations[Actor::State::Walk].addFrameSet(frames.getFramesFor("walk"));
	data[Actor::Type::Zombie4].animations[Actor::State::Walk].setDuration(sf::seconds(1.f));
	data[Actor::Type::Zombie4].animations[Actor::State::Walk].setRepeating(true);
	data[Actor::Type::Zombie4].animations[Actor::State::Dead].addFrameSet(frames.getFramesFor("dead"));
	data[Actor::Type::Zombie4].animations[Actor::State::Dead].setDuration(sf::seconds(1.f));
	data[Actor::Type::Zombie4].animations[Actor::State::Dead].setRepeating(false);
	data[Actor::Type::Zombie4].animations[Actor::State::Rise].addFrameSet(frames.getFramesFor("rise"));
	data[Actor::Type::Zombie4].animations[Actor::State::Rise].setDuration(sf::seconds(2.f));
	data[Actor::Type::Zombie4].animations[Actor::State::Rise].setRepeating(false);

	data[Actor::Type::Zombie4].directions.emplace_back(Direction(45.f, 50.f));
	data[Actor::Type::Zombie4].directions.emplace_back(Direction(-45.f, 100.f));
	data[Actor::Type::Zombie4].directions.emplace_back(Direction(45.f, 50.f));


	//zombie 5
	data[Actor::Type::Zombie5].texture = Textures::ID::Zombie5;
	data[Actor::Type::Zombie5].hitPoints = 100;
	data[Actor::Type::Zombie5].damageDone = 1;
	data[Actor::Type::Zombie5].speed = 50;
							
	frames = JsonFrameParser("Media/Textures/zombie5.json");
							
	data[Actor::Type::Zombie5].animations[Actor::State::Idle].addFrameSet(frames.getFramesFor("idle"));
	data[Actor::Type::Zombie5].animations[Actor::State::Idle].setDuration(sf::seconds(1.f));
	data[Actor::Type::Zombie5].animations[Actor::State::Idle].setRepeating(true);
	data[Actor::Type::Zombie5].animations[Actor::State::Attack].addFrameSet(frames.getFramesFor("attack"));
	data[Actor::Type::Zombie5].animations[Actor::State::Attack].setDuration(sf::seconds(1.f));
	data[Actor::Type::Zombie5].animations[Actor::State::Attack].setRepeating(true);
	data[Actor::Type::Zombie5].animations[Actor::State::Walk].addFrameSet(frames.getFramesFor("walk"));
	data[Actor::Type::Zombie5].animations[Actor::State::Walk].setDuration(sf::seconds(1.f));
	data[Actor::Type::Zombie5].animations[Actor::State::Walk].setRepeating(true);
	data[Actor::Type::Zombie5].animations[Actor::State::Dead].addFrameSet(frames.getFramesFor("dead"));
	data[Actor::Type::Zombie5].animations[Actor::State::Dead].setDuration(sf::seconds(1.f));
	data[Actor::Type::Zombie5].animations[Actor::State::Dead].setRepeating(false);
	data[Actor::Type::Zombie5].animations[Actor::State::Rise].addFrameSet(frames.getFramesFor("rise"));
	data[Actor::Type::Zombie5].animations[Actor::State::Rise].setDuration(sf::seconds(2.f));
	data[Actor::Type::Zombie5].animations[Actor::State::Rise].setRepeating(false);
							
	data[Actor::Type::Zombie5].directions.emplace_back(Direction(45.f, 50.f));
	data[Actor::Type::Zombie5].directions.emplace_back(Direction(-45.f, 100.f));
	data[Actor::Type::Zombie5].directions.emplace_back(Direction(45.f, 50.f));

	return data;
}							
