#pragma once
#include <SFML/Graphics.hpp>
#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"

#include "World.h"
#include "Player.h"
#include "Command.h"
#include "CommandQueue.h"
class Game
{

public:
						Game();//tyufu
	void				run();
private:
	void				processInput();
	void				update(sf::Time dt);
	void				render();

	void				handlePlayerInput(sf::Keyboard::Key key, bool isPressed);

	void				updateStatistics(sf::Time et);
	void				border();

private:
	sf::RenderWindow	window;
	World				world;
	Player				player;


	sf::Font			font;
	sf::Text			statsText;
	sf::Time			statsUpdateTime;
	std::size_t			statsNumFrames;
};

