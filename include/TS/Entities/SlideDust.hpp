#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <BA/Entities/Entity.hpp>
#include <BA/Components/Animation.hpp>
#include <BA/Components/Sprite.hpp>
#include <BA/Utilities/Rect.hpp>
#include <BA/Types.hpp>

namespace TS {

class SlideDust : public ba::Entity {
public:
	SlideDust(ba::SharedContext* context);

private:
	void loadResources();
	void populateAnimation();

private:
	static bool s_resourcesLoaded;
	static const std::unordered_map<ba::IDtype, std::pair<float, std::vector<std::string>>> s_gfxToLoad;
	static std::unordered_map<ba::IDtype, std::pair<float, std::vector<ba::IDtype>>> s_FXR;
}; // class SlideDust

} // namespace TS