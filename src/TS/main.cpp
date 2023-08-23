#include "TS/Entities/Hero.hpp"
#include "TS/Scenes/DemoLevel.hpp"

#include <BA/Engine.hpp>
#include <BA/Scenes/BitAlchemySplash.hpp>

namespace TS {

class TSEngine : public ba::Engine {
public:
	TSEngine();
	TSEngine(const std::string& title, ba::IntRect dimension, ba::uint32 winFlags);

	void init();
private:

}; // class TSEngine

TSEngine::TSEngine() :
	ba::Engine::Engine()
{

}

TSEngine::TSEngine(const std::string& title, ba::IntRect dimension, ba::uint32 winFlags) :
	ba::Engine::Engine(title, dimension, winFlags)
{

}

void TSEngine::init() {
	ba::Engine::init();

	std::shared_ptr<ba::BitAlchemySplash> bitAlchemySplash = std::make_shared<ba::BitAlchemySplash>(&m_window, &m_resources, &m_sceneManager);

	std::shared_ptr<TS::DemoLevel> demoLevel = std::make_shared<TS::DemoLevel>(&m_window, &m_resources, &m_sceneManager);

	m_sceneManager.add(bitAlchemySplash);
	IDtype demoLevelID = m_sceneManager.add(demoLevel);

	bitAlchemySplash->setSwitchTo(demoLevelID);
}

} // namespace TS


int main() {
	ba::debug.setColor(ba::Color::Green);

	TS::TSEngine TimeShifters("Time Shifters Demo", ba::IntRect{64, 64, 962, 768}, SDL_WINDOW_SHOWN | SDL_WINDOW_MAXIMIZED);
	TimeShifters.init();
	TimeShifters.run();
	TimeShifters.cleanUp();
}