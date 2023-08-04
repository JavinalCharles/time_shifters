#include "TS/Utility/Define.hpp"

namespace TS {

namespace {
	bool SFX_LOADED = false;
} // namespace anonymous

const std::unordered_map<IDtype, std::string> g_SFX_MAP{
	{CONCRETE_1, 	"Footsteps/Concrete 1.wav"},
	{CONCRETE_2, 	"Footsteps/Concrete 2.wav"},
	{FOREST_1, 		"Footsteps/Forest 1.wav"},
	{FOREST_2, 		"Footsteps/Forest 2.wav"},
	{GRASS_1, 		"Footsteps/Grass 1.wav"},
	{GRASS_2, 		"Footsteps/Grass Running.wav"},
	{GRAVEL_1, 		"Footsteps/Gravel 1.wav"},
	{GRAVEL_2, 		"Footsteps/Gravel - Run.wav"},
	{SAND, 			"Footsteps/Sand.wav"},
	{SNOW, 			"Footsteps/Snow.wav"},
};

std::unordered_map<IDtype, IDtype> g_SFXR = {};

void loadGlobalSounds(ba::ResourceManager* r_manager) {
	if (SFX_LOADED) {
		return;
	}

	for (auto& [id, str] : g_SFX_MAP) {
		g_SFXR.insert_or_assign(id, r_manager->loadSound(str));
	}

	SFX_LOADED = true;
}

} // namespace TS