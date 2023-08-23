#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <BA/Entities/Entity.hpp>
#include <BA/Components/Animation.hpp>
#include <BA/Components/Camera.hpp>
#include <BA/Components/Colliders/BoxCollider.hpp>
#include <BA/Components/KeyboardControl.hpp>
#include <BA/Components/MouseControl.hpp>
#include <BA/Components/SoundEmitter.hpp>
#include <BA/Components/Sprite.hpp>
#include <BA/Components/Timer.hpp>
#include <BA/Components/Velocity.hpp>
#include <BA/Utilities/Vector2.hpp>
#include <BA/Systems/EntityManager.hpp>
#include <BA/Systems/VelocityWithCollisionSystem.hpp>
#include <BA/Types.hpp>
#include <BA/Tools/DebugHelper.hpp>

#include <SDL2/SDL_keycode.h>

#include "TS/Components/AttackBoxCollider.hpp"
#include "TS/Components/AttackCollidable.hpp"
#include "TS/Components/ModifiedBoxCollider.hpp"
#include "TS/Components/Updateable.hpp"
#include "TS/Entities/Character.hpp"
#include "TS/Entities/SlideDust.hpp"
#include "TS/Utility/Define.hpp"

using ba::Entity;
using ba::IDtype;
using ba::SharedContext;

namespace TS {

class Hero : public Character {
public:
	explicit Hero(SharedContext* context);

	void updatePreviousPosition(float deltaTime);
	const ba::Vector2f& getPreviousPosition() const;

private:
	void setMouseButtonBindings(std::shared_ptr<ba::MouseControl>& mc);
	void setKeyBindings(std::shared_ptr<ba::KeyboardControl>& kc);
	void populateAnimation(std::shared_ptr<ba::Animation>& a);
	void loadResources();

	void spawnDust();
	void attack();
	// void spawnAttackLeft(float secondsToDie);
private:
	ba::Vector2f m_thisPosition;
	ba::Vector2f m_previousPosition;

	bool m_jumping = false;
	bool m_doubleJumped = false;
	float m_jumpedTime = 0.0f;

	static const float s_MAX_JUMP_TIME;
private:
	static bool s_resourcesLoaded;
	static const std::unordered_map<IDtype, std::pair<float, std::vector<std::string>>> s_resourcesToLoad;
	static std::unordered_map<IDtype, std::pair<float,std::vector<IDtype>>> s_R;

}; // class Hero

} // namespace TS