#include "TS/Systems/UpdateableSystem.hpp"
#include <BA/Systems/ComponentSystem.hpp>
#include <BA/Systems/EntityManager.hpp>
#include <BA/Entities/Entity.hpp>

namespace TS {

UpdateableSystem::UpdateableSystem(ba::EntityManager* entityManager) :
	ba::ComponentSystem::ComponentSystem(entityManager)
{

}

void UpdateableSystem::update(float deltaTime) {
	for (ba::IDtype ID : this->getIDs()) {
		auto& updates = this->getEntity(ID)->getComponent<Updateable>()->getUpdates();
		for (auto& update : updates) {
			update(deltaTime);
		}
	}
}

void UpdateableSystem::add(std::shared_ptr<ba::Entity>& entity) {
	if (entity->getComponent<Updateable>() != nullptr) {
		this->addID(entity->ID);
	}
}

} // namespace TS