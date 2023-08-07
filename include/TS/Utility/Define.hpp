#pragma once

#include <string>
#include <unordered_map>
#include <BA/ResourceManager.hpp>
#include <BA/Types.hpp>

using ba::IDtype;

namespace TS {

const float Y_GRAVITY = 512.f;
const float NORMAL_SPEED = 256.f;
const float SLOW_SPEED = 64.f;

enum CollisionLayers : IDtype {
	TILE_1 = 0u,
	TILE_2,
	TILE_3,
	OBJECTS_1,
	OBJECTS_2,
	OBJECTS_3,
	VISION_1,
	VISION_2,
	VISION_3,
	NPC_1,
	NPC_2,
	NPC_3,
	NPC_ATTACK_1,
	NPC_ATTACK_2,
	NPC_ATTACK_3,
	PLAYER_1,
	PLAYER_2,
	PLAYER_3,
	PLAYER_ATTACK_1,
	PLAYER_ATTACK_2,
	PLAYER_ATTACK_3
};

enum SoundEffects : IDtype{
	SILENT = 0,
	CONCRETE_1,
	CONCRETE_2,
	FOREST_1,
	FOREST_2,
	GRASS_1,
	GRASS_2,
	GRAVEL_1,
	GRAVEL_2,
	SAND,
	SNOW,
};

extern const std::unordered_map<IDtype, std::string> g_SFX_MAP;

extern std::unordered_map<IDtype, IDtype> g_SFXR;

void loadGlobalSounds(ba::ResourceManager* r_manager);

} // namespace TS