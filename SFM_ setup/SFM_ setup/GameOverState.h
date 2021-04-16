#pragma once
#include "State.h"
#include "Container.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


class GameOverState : public State
{
public:
	GameOverState(StateStack& stack, Context context);

	virtual void		draw();
	virtual bool		update(sf::Time dt);
	virtual bool		handleEvent(const sf::Event& event);


private:
	sf::Time			mTextEffectTime;
	bool				mShowText;
	sf::Text			mText;
	sf::Text			mGameOverText;
	sf::Time			mElapsedTime;
};

