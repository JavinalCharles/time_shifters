#include "TS/Components/Updateable.hpp"
#include <BA/Entities/Entity.hpp>

namespace TS {

const ba::IDtype Updateable::CID = ba::ComponentID::XTRA1;

Updateable::Updateable(ba::Entity* owner) :
	ba::Component::Component(owner)
{

}

void Updateable::addUpdate(Update& update) {
	m_updates.push_back(update);
}

const std::vector<Update>& Updateable::getUpdates() const {
	return m_updates;
}

} // namespace TS