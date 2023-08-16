#pragma once

#include <BA/Components/Text.hpp>

namespace TS {

class ScreenText : public ba::Text {
public:
	ScreenText(ba::Entity* owner);

	virtual void draw(ba::Window& window) override;
private:
}; // class ScreenText

} // namespace TS