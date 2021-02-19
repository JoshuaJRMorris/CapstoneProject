#pragma once
#include <memory>
#include <map>
#include "SFML/Graphics.hpp"
#include <string>
enum class Textures {Landscape, Airplane, Missle};
class TextureHolder
{
public:
	void load(Textures id, const std::string& filename);

	const sf::Texture& get(Textures id) const;
private:
	std::map<Textures, std::unique_ptr<sf::Texture>> textureMap;
};

