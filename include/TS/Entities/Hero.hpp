#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <BA/Entities/Entity.hpp>
#include <BA/Components/Animation.hpp>
#include <BA/Components/Camera.hpp>
#include <BA/Components/Colliders/BoxCollider.hpp>
#include <BA/Components/KeyboardControl.hpp>
#include <BA/Components/MouseControl.hpp>
#include <BA/Components/Sprite.hpp>
#include <BA/Components/Velocity.hpp>
#include <BA/Utilities/Vector2.hpp>
#include <BA/Types.hpp>

#include <SDL2/SDL_keycode.h>

#include "TS/Components/ModifiedBoxCollider.hpp"
#include "TS/Components/Updateable.hpp"

using ba::Entity;
using ba::IDtype;
using ba::SharedContext;

namespace TS {

class Hero : public Entity {
public:
	Hero(SharedContext* context);

	void updatePreviousPosition(float deltaTime);
	const ba::Vector2f& getPreviousPosition() const;


private:
	void setMouseButtonBindings(std::shared_ptr<ba::MouseControl>& mc);
	void setKeyBindings(std::shared_ptr<ba::KeyboardControl>& kc);
	void populateAnimation(std::shared_ptr<ba::Animation>& a);
	void loadResources();

private:
	ba::Vector2f m_previousPosition;
	ba::Vector2f m_thisPosition;

private:
	static bool s_resourcesLoaded;
	static const std::unordered_map<IDtype, std::pair<float, std::vector<std::string>>> s_resourcesToLoad;
	static std::unordered_map<IDtype, std::pair<float,std::vector<IDtype>>> s_R;

}; // class Hero

} // namespace TS