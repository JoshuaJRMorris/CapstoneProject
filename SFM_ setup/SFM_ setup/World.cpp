#include "World.h"
#include "Projectile.h"
#include "Pickup.h"
#include "TextNode.h"
#include "ParticleNode.h"
#include "SoundNode.h"

#include <SFML/Graphics/RenderTarget.hpp>

#include <algorithm>
#include <cmath>
#include <limits>


World::World(sf::RenderTarget& outputTarget, FontHolder& fonts, SoundPlayer& sounds)
	: mTarget(outputTarget)
	, mSceneTexture()
	, mWorldView(outputTarget.getDefaultView())
	, mTextures()
	, mFonts(fonts)
	, mSounds(sounds)
	, mSceneGraph()
	, mSceneLayers()
	, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, 5000.f)
	, mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldBounds.height - mWorldView.getSize().y / 2.f)
	, mScrollSpeed(-50.f)
	, mPlayerAircraft(nullptr)
	, mEnemySpawnPoints()
	, mActiveEnemies()
{
	mSceneTexture.create(mTarget.getSize().x, mTarget.getSize().y);

	loadTextures();
	buildScene();

	// Prepare the view
	mWorldView.setCenter(mSpawnPosition);
}

void World::update(sf::Time dt)
{
	// Scroll the world, reset player velocity
	//mWorldView.move(0.f, mScrollSpeed * dt.asSeconds());
	mPlayerAircraft->setVelocity(0.f, 0.f);

	// Setup commands to destroy entities, and guide missiles
	destroyEntitiesOutsideView();
	adaptNPCPosition();
	//guideMissiles();

	// Forward commands to scene graph, adapt velocity (scrolling, diagonal correction)
	while (!mCommandQueue.isEmpty())
		mSceneGraph.onCommand(mCommandQueue.pop(), dt);
	adaptPlayerVelocity();

	// Collision detection and response (may destroy entities)
	handleCollisions();

	// Remove all destroyed entities, create new ones
	mSceneGraph.removeWrecks();
	spawnEnemies();

	// Regular update step, adapt position (correct if outside view)
	mSceneGraph.update(dt, mCommandQueue);
	adaptPlayerPosition();

	updateSounds();
}

void World::draw()
{
	if (PostEffect::isSupported())
	{
		mSceneTexture.clear();
		mSceneTexture.setView(mWorldView);
		mSceneTexture.draw(mSceneGraph);
		mSceneTexture.display();
		mBloomEffect.apply(mSceneTexture, mTarget);
	}
	else
	{
		mTarget.setView(mWorldView);
		mTarget.draw(mSceneGraph);
	}
}

CommandQueue& World::getCommandQueue()
{
	return mCommandQueue;
}

bool World::hasAlivePlayer() const
{
	return !mPlayerAircraft->isMarkedForRemoval();
}

bool World::hasPlayerReachedEnd() const
{
	return !mWorldBounds.contains(mPlayerAircraft->getPosition());
}

void World::loadTextures()
{
	/*mTextures.load(Textures::Entities, "Media/Textures/Entities.png");
	mTextures.load(Textures::Jungle, "Media/Textures/Jungle.png");
	mTextures.load(Textures::Explosion, "Media/Textures/Explosion.png");
	mTextures.load(Textures::Particle, "Media/Textures/Particle.png");
	mTextures.load(Textures::FinishLine, "Media/Textures/FinishLine.png");*/

	mTextures.load(Textures::Road, "Media/Textures/road.png");
	mTextures.load(Textures::Zombie1, "Media/Textures/zombie1.png");
	mTextures.load(Textures::Zombie2, "Media/Textures/zombie2.png");
	mTextures.load(Textures::Zombie3, "Media/Textures/zombie3.png");
	mTextures.load(Textures::Zombie4, "Media/Textures/zombie4.png");
	mTextures.load(Textures::Zombie5, "Media/Textures/zombie5.png");
	mTextures.load(Textures::Hero2, "Media/Textures/hero2.png");



}

void World::adaptPlayerPosition()
{
	// Keep player's position inside the screen bounds, at least borderDistance units from the border
	sf::FloatRect viewBounds = getViewBounds();
	const float borderDistance = 40.f;

	sf::Vector2f position = mPlayerAircraft->getPosition();
	position.x = std::max(position.x, viewBounds.left + borderDistance);
	position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance);
	position.y = std::max(position.y, viewBounds.top + borderDistance);
	position.y = std::min(position.y, viewBounds.top + viewBounds.height - borderDistance);
	mPlayerAircraft->setPosition(position);
}

void World::adaptNPCPosition()
{
	sf::FloatRect viewBounds(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
	const float borderDistance = 40.f;

	Command adaptPosition;
	adaptPosition.category = Category::Zombie;
	adaptPosition.action = derivedAction<Actor>([this](Actor& enemy, sf::Time)
		{
			/*float left = mWorldView.getCenter().x - mWorldView.getSize().x / 2.f;
			float right = mWorldView.getCenter().x + mWorldView.getSize().x / 2.f;
			float top = mWorldView.getCenter().y - mWorldView.getSize().y / 2.f;
			float bottom = mWorldView.getCenter().y + mWorldView.getSize().y / 2.f;*/

			sf::FloatRect viewBounds(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
			const float borderDistance = 40.f;

			sf::Vector2f position = enemy.getPosition();

			position.x = std::max(position.x, viewBounds.left + borderDistance);
			position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance);
			position.y = std::max(position.y, viewBounds.top + borderDistance);
			position.y = std::min(position.y, viewBounds.top + viewBounds.height - borderDistance);

			enemy.setPosition(position);
		});

	mCommandQueue.push(adaptPosition);
}

void World::adaptPlayerVelocity()
{
	sf::Vector2f velocity = mPlayerAircraft->getVelocity();

	// If moving diagonally, reduce velocity (to have always same velocity)
	if (velocity.x != 0.f && velocity.y != 0.f)
		mPlayerAircraft->setVelocity(velocity / std::sqrt(2.f));

	// Add scrolling velocity
	
}

bool matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2)
{
	unsigned int category1 = colliders.first->getCategory();
	unsigned int category2 = colliders.second->getCategory();

	// Make sure first pair entry has category type1 and second has type2
	if (type1 & category1 && type2 & category2)
	{
		return true;
	}
	else if (type1 & category2 && type2 & category1)
	{
		std::swap(colliders.first, colliders.second);
		return true;
	}
	else
	{
		return false;
	}
}

void World::handleCollisions()
{
	std::set<SceneNode::Pair> collisionPairs;
	mSceneGraph.checkSceneCollision(mSceneGraph, collisionPairs);

	for (auto pair : collisionPairs)
	{
		if (matchesCategories(pair, Category::Hero, Category::Zombie)) {
			auto& hero = static_cast<Actor&>(*pair.first);
			auto& zombie = static_cast<Actor&>(*pair.second);

			if (hero.getState() == Actor::State::Attack)
				zombie.damage(1);
			else
				hero.damage(1);


		}

		if (matchesCategories(pair, Category::Zombie, Category::Zombie)) {
			auto& first = static_cast<Actor&>(*pair.first);
			auto& second = static_cast<Actor&>(*pair.second);

			if (first.getPosition().x < second.getPosition().x) {
				first.move(sf::Vector2f(-1.f, 0.f));
				second.move(sf::Vector2f(1.f, 0.f));
			}
			else {
				first.move(sf::Vector2f(1.f, 0.f));
				second.move(sf::Vector2f(-1.f, 0.f));
			}

			if (first.getPosition().y < second.getPosition().y) {
				first.move(sf::Vector2f(0.f, -1.f));
				second.move(sf::Vector2f(0.f, 1.f));
			}
			else {
				first.move(sf::Vector2f(0.f, 1.f));
				second.move(sf::Vector2f(0.f, -1.f));
			}
		}

		if (matchesCategories(pair, Category::Zombie, Category::Hero)) {
			auto& first = static_cast<Actor&>(*pair.first);
			auto& second = static_cast<Actor&>(*pair.second);

			if (first.getPosition().x < second.getPosition().x) {
				first.move(sf::Vector2f(-1.f, 0.f));
				second.move(sf::Vector2f(1.f, 0.f));
			}
			else {
				first.move(sf::Vector2f(1.f, 0.f));
				second.move(sf::Vector2f(-1.f, 0.f));
			}

			if (first.getPosition().y < second.getPosition().y) {
				first.move(sf::Vector2f(0.f, -1.f));
				second.move(sf::Vector2f(0.f, 1.f));
			}
			else {
				first.move(sf::Vector2f(0.f, 1.f));
				second.move(sf::Vector2f(0.f, -1.f));
			}
		}
	}
}

void World::updateSounds()
{
	// Set listener's position to player position
	mSounds.setListenerPosition(mPlayerAircraft->getWorldPosition());

	// Remove unused sounds
	mSounds.removeStoppedSounds();
}

void World::buildScene()
{
	// Initialize the different layers
	for (std::size_t i = 0; i < LayerCount; ++i)
	{
		Category::Type category = (i == LowerAir) ? Category::SceneAirLayer : Category::None;

		SceneNode::Ptr layer(new SceneNode(category));
		mSceneLayers[i] = layer.get();

		mSceneGraph.attachChild(std::move(layer));
	}

	// Prepare the tiled background
	sf::Texture& roadTexture = mTextures.get(Textures::Road);
	roadTexture.setRepeated(true);

	float viewHeight = mWorldView.getSize().y;
	sf::IntRect textureRect(mWorldBounds);
	textureRect.height += static_cast<int>(viewHeight);

	// Add the background sprite to the scene
	std::unique_ptr<SpriteNode> road(new SpriteNode(roadTexture, textureRect));
	road->setPosition(mWorldBounds.left, mWorldBounds.top - viewHeight);
	mSceneLayers[Background]->attachChild(std::move(road));

	// Add the finish line to the scene
	/*sf::Texture& finishTexture = mTextures.get(Textures::FinishLine);
	std::unique_ptr<SpriteNode> finishSprite(new SpriteNode(finishTexture));
	finishSprite->setPosition(0.f, -76.f);
	mSceneLayers[Background]->attachChild(std::move(finishSprite));*/

	// Add particle node to the scene
	/*std::unique_ptr<ParticleNode> smokeNode(new ParticleNode(Particle::Smoke, mTextures));
	mSceneLayers[LowerAir]->attachChild(std::move(smokeNode));*/

	// Add propellant particle node to the scene
	/*std::unique_ptr<ParticleNode> propellantNode(new ParticleNode(Particle::Propellant, mTextures));
	mSceneLayers[LowerAir]->attachChild(std::move(propellantNode));*/

	// Add sound effect node
	std::unique_ptr<SoundNode> soundNode(new SoundNode(mSounds));
	mSceneGraph.attachChild(std::move(soundNode));

	// Add player's aircraft
	std::unique_ptr<Actor> player(new Actor(Actor::Type::Hero2, mTextures, mFonts));
	mPlayerAircraft = player.get();
	mPlayerAircraft->setPosition(mSpawnPosition);
	mSceneLayers[UpperAir]->attachChild(std::move(player));

	// Add enemy aircraft
	addEnemies();
}

void World::addEnemies()
{
	// Add enemies to the spawn point container
	addEnemy(Actor::Type::Zombie1, 0.f, 300.f);
	addEnemy(Actor::Type::Zombie2, -170.f, 200.f);
	addEnemy(Actor::Type::Zombie4, +170.f, 200.f);
	addEnemy(Actor::Type::Zombie3, 0.f, 100.f);
	addEnemy(Actor::Type::Zombie5, -170.f, 100.f);
	addEnemy(Actor::Type::Zombie1, 170.f, 100.f);

	//addEnemy(Aircraft::Avenger, -70.f, 1710.f);
	//addEnemy(Aircraft::Avenger, 70.f, 1700.f);
	//addEnemy(Aircraft::Avenger, 30.f, 1850.f);
	//addEnemy(Aircraft::Raptor, 300.f, 2200.f);
	//addEnemy(Aircraft::Raptor, -300.f, 2200.f);
	//addEnemy(Aircraft::Raptor, 0.f, 2200.f);
	//addEnemy(Aircraft::Raptor, 0.f, 2500.f);
	//addEnemy(Aircraft::Avenger, -300.f, 2700.f);
	//addEnemy(Aircraft::Avenger, -300.f, 2700.f);
	//addEnemy(Aircraft::Raptor, 0.f, 3000.f);
	//addEnemy(Aircraft::Raptor, 250.f, 3250.f);
	//addEnemy(Aircraft::Raptor, -250.f, 3250.f);
	//addEnemy(Aircraft::Avenger, 0.f, 3500.f);
	//addEnemy(Aircraft::Avenger, 0.f, 3700.f);
	//addEnemy(Aircraft::Raptor, 0.f, 3800.f);
	//addEnemy(Aircraft::Avenger, 0.f, 4000.f);
	//addEnemy(Aircraft::Avenger, -200.f, 4200.f);
	//addEnemy(Aircraft::Raptor, 200.f, 4200.f);
	//addEnemy(Aircraft::Raptor, 0.f, 4400.f);


	// Sort all enemies according to their y value, such that lower enemies are checked first for spawning
	std::sort(mEnemySpawnPoints.begin(), mEnemySpawnPoints.end(), [](SpawnPoint lhs, SpawnPoint rhs)
		{
			return lhs.y < rhs.y;
		});
}

void World::addEnemy(Actor::Type type, float relX, float relY)
{
	SpawnPoint spawn(type, mSpawnPosition.x + relX, mSpawnPosition.y - relY);
	mEnemySpawnPoints.push_back(spawn);
}

void World::spawnEnemies()
{
	// Spawn all enemies entering the view area (including distance) this frame
	while (!mEnemySpawnPoints.empty()
		&& mEnemySpawnPoints.back().y > getBattlefieldBounds().top)
	{
		SpawnPoint spawn = mEnemySpawnPoints.back();

		std::unique_ptr<Actor> enemy(new Actor(spawn.type, mTextures, mFonts));
		enemy->setPosition(spawn.x, spawn.y);
		//enemy->setRotation(180.f);

		mSceneLayers[UpperAir]->attachChild(std::move(enemy));

		// Enemy is spawned, remove from the list to spawn
		mEnemySpawnPoints.pop_back();
	}
}

void World::destroyEntitiesOutsideView()
{
	Command command;
	command.category = Category::Projectile | Category::EnemyAircraft;
	command.action = derivedAction<Entity>([this](Entity& e, sf::Time)
		{
			if (!getBattlefieldBounds().intersects(e.getBoundingRect()))
				e.remove();
		});

	mCommandQueue.push(command);
}

//void World::guideMissiles()
//{
//	// Setup command that stores all enemies in mActiveEnemies
//	Command enemyCollector;
//	enemyCollector.category = Category::EnemyAircraft;
//	enemyCollector.action = derivedAction<Actor>([this](Actor& enemy, sf::Time)
//		{
//			if (!enemy.isDestroyed())
//				mActiveEnemies.push_back(&enemy);
//		});
//
//	// Setup command that guides all missiles to the enemy which is currently closest to the player
//	Command missileGuider;
//	missileGuider.category = Category::AlliedProjectile;
//	missileGuider.action = derivedAction<Projectile>([this](Projectile& missile, sf::Time)
//		{
//			// Ignore unguided bullets
//			if (!missile.isGuided())
//				return;
//
//			float minDistance = std::numeric_limits<float>::max();
//			Actor* closestEnemy = nullptr;
//
//			// Find closest enemy
//			for (Actor * enemy : mActiveEnemies)
//			{
//				float enemyDistance = distance(missile, *enemy);
//
//				if (enemyDistance < minDistance)
//				{
//					closestEnemy = enemy;
//					minDistance = enemyDistance;
//				}
//			}
//
//			if (closestEnemy)
//				missile.guideTowards(closestEnemy->getWorldPosition());
//		});
//
//	// Push commands, reset active enemies
//	mCommandQueue.push(enemyCollector);
//	mCommandQueue.push(missileGuider);
//	mActiveEnemies.clear();
//}

sf::FloatRect World::getViewBounds() const
{
	return sf::FloatRect(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
}

sf::FloatRect World::getBattlefieldBounds() const
{
	// Return view bounds + some area at top, where enemies spawn
	sf::FloatRect bounds = getViewBounds();
	bounds.top -= 100.f;
	bounds.height += 100.f;

	return bounds;
}