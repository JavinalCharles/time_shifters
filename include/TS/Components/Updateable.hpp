#pragma once 

#include <vector>
#include <functional>
#include <BA/Components/Component.hpp>

namespace TS {

using Update = std::function<void(float)>;

class Updateable : public ba::Component {
public:
	explicit Updateable(ba::Entity* owner);

	void addUpdate(Update& update);
	const std::vector<Update>& getUpdates() const;
private:
	std::vector<Update> m_updates;

public:
	static const ba::IDtype CID;
}; // class Updateable

} // namespace TS