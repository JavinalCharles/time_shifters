#pragma once

#include "TS/Entities/Hero.hpp"

#include <BA/Entities/SharedContext.hpp>
#include <BA/Generator/TileMapGenerator.hpp>
#include <BA/Scenes/Scene.hpp>
#include <BA/Systems/AnimationSystem.hpp>
#include <BA/Systems/CameraSystem.hpp>
#include <BA/Systems/CollisionSystem.hpp>
#include <BA/Systems/EntityManager.hpp>
#include <BA/Systems/KeyboardControlSystem.hpp>
#include <BA/Systems/MouseControlSystem.hpp>
#include <BA/Systems/MovementSystem.hpp>

namespace TS {

class DemoLevel : public ba::Scene {
public:
	DemoLevel();
	DemoLevel(ba::Window* window, ba::ResourceManager* resources, ba::SceneManager* sceneManager);

	virtual void onCreate();
	virtual void onDestroy();

	virtual void onActivate() override;

	virtual void handleEvents() override;
	virtual void update(float deltaTime) override;
	virtual void postUpdate(float deltaTime) override;

	virtual void draw(ba::Window& window);
private:
	void updateBackground(ba::Window& window);
	void generateBackground();
	void generateMap();
	void createHero();

private:
	ba::EntityManager	m_entityManager;
	ba::InputManager 	m_inputManager;
	ba::MusicPlayer 	m_musicPlayer;

	std::vector<std::shared_ptr<Entity>> m_rollingBackgrounds;
}; // class DemoLevel

} // namespace TS