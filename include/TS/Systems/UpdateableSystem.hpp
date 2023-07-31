#pragma once

#include <BA/Systems/ComponentSystem.hpp>
#include "TS/Components/Updateable.hpp"

namespace TS {

class UpdateableSystem : public ba::ComponentSystem {
public:
	UpdateableSystem(ba::EntityManager* entityManager);

	virtual void update(float deltaTime);

	virtual void add(std::shared_ptr<ba::Entity>& entity);

private:

}; // class UpdateableSystem

} // namespace TS