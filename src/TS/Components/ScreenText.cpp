#include "TS/Components/ScreenText.hpp"


namespace TS {

ScreenText::ScreenText(ba::Entity* owner) :
	ba::Text::Text(owner)
{

}


void ScreenText::draw(ba::Window& window) {
	window.drawOnScreen(this->getTexture(), this->getGlobalBounds());
}

} // namespace TS