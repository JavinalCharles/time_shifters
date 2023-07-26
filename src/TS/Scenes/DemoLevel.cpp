#include "TS/Scenes/DemoLevel.hpp"

namespace TS {

DemoLevel::DemoLevel() :
	ba::Scene::Scene()
{
	m_CONTEXT.entities = &m_entityManager;
	m_CONTEXT.inputs = &m_inputManager;
	m_CONTEXT.player = &m_musicPlayer;
}

DemoLevel::DemoLevel(ba::Window* window, ba::ResourceManager* resources, ba::SceneManager* sceneManager) :
	ba::Scene::Scene(window, resources, sceneManager)
{
	m_CONTEXT.entities = &m_entityManager;
	m_CONTEXT.inputs = &m_inputManager;
	m_CONTEXT.player = &m_musicPlayer;
}

void DemoLevel::onCreate() {
	m_CONTEXT.inputs->addInput<ba::KeyboardInput>();
	m_CONTEXT.inputs->addInput<ba::MouseInput>();

	m_entityManager.includeSystem<ba::AnimationSystem>();
	m_entityManager.includeSystem<ba::CameraSystem>();
	m_entityManager.includeSystem<ba::CollisionSystem>();
	m_entityManager.includeSystem<ba::KeyboardControlSystem>();
	m_entityManager.includeSystem<ba::MouseControlSystem>();
	m_entityManager.includeSystem<ba::MovementSystem>();
}

void DemoLevel::onDestroy() {

}


void DemoLevel::onActivate() {
	this->createHero();
}


void DemoLevel::handleEvents() {
	m_inputManager.handleEvents();
}

void DemoLevel::update(float deltaTime) {
	m_entityManager.update(deltaTime);
}

void DemoLevel::postUpdate(float deltaTime) {
	m_entityManager.postUpdate(deltaTime);
}

void DemoLevel::draw(ba::Window& window) {
	m_entityManager.draw(window);
}


void DemoLevel::createHero() {
	std::shared_ptr<Hero>  hero = std::make_shared<Hero>(&m_CONTEXT);

	m_entityManager.add(hero);
}

}// namespace TS