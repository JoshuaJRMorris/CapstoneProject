#include "DataTables.h"
#include "Actor.h"
#include "Projectile.h"
#include "Pickup.h"
#include "Particle.h"


// For std::bind() placeholders _1, _2, ...
using namespace std::placeholders;

std::vector<AircraftData> initializeAircraftData()
{
	std::vector<AircraftData> data(Actor::TypeCount);

	data[Actor::RedBird].hitpoints = 100;
	data[Actor::RedBird].speed = 400.f;
	data[Actor::RedBird].texture = Textures::RedBird;
	data[Actor::RedBird].textureRect = sf::IntRect(0, 0, 20, 20);
	data[Actor::RedBird].hasRollAnimation = false;

	data[Actor::BlueBird].hitpoints = 100;
	data[Actor::BlueBird].speed = 400.f;
	data[Actor::BlueBird].texture = Textures::BlueBird;
	data[Actor::BlueBird].textureRect = sf::IntRect(0, 0, 20, 20);
	data[Actor::BlueBird].hasRollAnimation = false;

	//Just for demo purposes

	data[Actor::GreyBird].hitpoints = 100;
	data[Actor::GreyBird].speed = 400.f;
	data[Actor::GreyBird].texture = Textures::GreyBird;
	data[Actor::GreyBird].textureRect = sf::IntRect(0, 0, 20, 20);
	data[Actor::GreyBird].hasRollAnimation = false;


	data[Actor::Eagle].hitpoints = 100;
	data[Actor::Eagle].speed = 200.f;
	data[Actor::Eagle].fireInterval = sf::seconds(1);
	data[Actor::Eagle].texture = Textures::Entities;
	data[Actor::Eagle].textureRect = sf::IntRect(0, 0, 48, 64);
	data[Actor::Eagle].hasRollAnimation = true;

	data[Actor::Raptor].hitpoints = 20;
	data[Actor::Raptor].speed = 80.f;
	data[Actor::Raptor].texture = Textures::Entities;
	data[Actor::Raptor].textureRect = sf::IntRect(144, 0, 84, 64);
	data[Actor::Raptor].directions.push_back(Direction(+45.f, 80.f));
	data[Actor::Raptor].directions.push_back(Direction(-45.f, 160.f));
	data[Actor::Raptor].directions.push_back(Direction(+45.f, 80.f));
	data[Actor::Raptor].fireInterval = sf::Time::Zero;
	data[Actor::Raptor].hasRollAnimation = false;

	data[Actor::Avenger].hitpoints = 40;
	data[Actor::Avenger].speed = 50.f;
	data[Actor::Avenger].texture = Textures::Entities;
	data[Actor::Avenger].textureRect = sf::IntRect(228, 0, 60, 59);
	data[Actor::Avenger].directions.push_back(Direction(+45.f, 50.f));
	data[Actor::Avenger].directions.push_back(Direction(0.f, 50.f));
	data[Actor::Avenger].directions.push_back(Direction(-45.f, 100.f));
	data[Actor::Avenger].directions.push_back(Direction(0.f, 50.f));
	data[Actor::Avenger].directions.push_back(Direction(+45.f, 50.f));
	data[Actor::Avenger].fireInterval = sf::seconds(2);
	data[Actor::Avenger].hasRollAnimation = false;

	return data;
}

//probably won't need anything below this
std::vector<ProjectileData> initializeProjectileData()
{
	std::vector<ProjectileData> data(Projectile::TypeCount);

	data[Projectile::AlliedBullet].damage = 10;
	data[Projectile::AlliedBullet].speed = 300.f;
	data[Projectile::AlliedBullet].texture = Textures::Entities;
	data[Projectile::AlliedBullet].textureRect = sf::IntRect(175, 64, 3, 14);

	data[Projectile::EnemyBullet].damage = 10;
	data[Projectile::EnemyBullet].speed = 300.f;
	data[Projectile::EnemyBullet].texture = Textures::Entities;
	data[Projectile::EnemyBullet].textureRect = sf::IntRect(178, 64, 3, 14);

	data[Projectile::Missile].damage = 200;
	data[Projectile::Missile].speed = 150.f;
	data[Projectile::Missile].texture = Textures::Entities;
	data[Projectile::Missile].textureRect = sf::IntRect(160, 64, 15, 32);

	return data;
}

std::vector<PickupData> initializePickupData()
{
	std::vector<PickupData> data(Pickup::TypeCount);

	data[Pickup::HealthRefill].texture = Textures::Entities;
	data[Pickup::HealthRefill].textureRect = sf::IntRect(0, 64, 40, 40);
	data[Pickup::HealthRefill].action = [](Actor& a) { a.repair(25); };

	data[Pickup::MissileRefill].texture = Textures::Entities;
	data[Pickup::MissileRefill].textureRect = sf::IntRect(40, 64, 40, 40);
	data[Pickup::MissileRefill].action = std::bind(&Actor::collectMissiles, _1, 3);

	data[Pickup::FireSpread].texture = Textures::Entities;
	data[Pickup::FireSpread].textureRect = sf::IntRect(80, 64, 40, 40);
	data[Pickup::FireSpread].action = std::bind(&Actor::increaseSpread, _1);

	data[Pickup::FireRate].texture = Textures::Entities;
	data[Pickup::FireRate].textureRect = sf::IntRect(120, 64, 40, 40);
	data[Pickup::FireRate].action = std::bind(&Actor::increaseFireRate, _1);

	return data;
}

std::vector<ParticleData> initializeParticleData()
{
	std::vector<ParticleData> data(Particle::ParticleCount);

	data[Particle::Propellant].color = sf::Color(255, 255, 50);
	data[Particle::Propellant].lifetime = sf::seconds(0.6f);

	data[Particle::Smoke].color = sf::Color(50, 50, 50);
	data[Particle::Smoke].lifetime = sf::seconds(4.f);

	return data;
}
