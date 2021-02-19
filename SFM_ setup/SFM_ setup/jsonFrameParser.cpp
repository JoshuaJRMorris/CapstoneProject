#include "jsonFrameParser.h"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

JsonFrameParser::JsonFrameParser(std::string path) {
	std::ifstream ifs(path);
	json_ = json::parse(ifs);
	//std::cout << json_.dump(6);
}

std::vector<sf::IntRect> JsonFrameParser::getFramesFor(std::string animationName) const {
	std::vector<sf::IntRect> data;

	json k = json_["frames"];

	for (auto i : k) {
		std::string tmpStr = i["filename"];
		if (tmpStr.compare(0, animationName.size(), animationName) == 0) {

			data.push_back(sf::IntRect(i["frame"]["x"],
				i["frame"]["y"],
				i["frame"]["w"],
				i["frame"]["h"] ));
		}
	}
	return data;
}