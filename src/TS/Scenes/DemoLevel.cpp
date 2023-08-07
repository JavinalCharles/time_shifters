#include "TS/Scenes/DemoLevel.hpp"

namespace TS {

DemoLevel::DemoLevel() :
	ba::Scene::Scene(),
	m_musicPlayer(nullptr),
	m_engine(std::chrono::system_clock::now().time_since_epoch().count()),
	m_distributor(0, 3)
{
	m_CONTEXT.entities = &m_entityManager;
	m_CONTEXT.inputs = &m_inputManager;
	m_CONTEXT.player = &m_musicPlayer;
}

DemoLevel::DemoLevel(ba::Window* window, ba::ResourceManager* resources, ba::SceneManager* sceneManager) :
	ba::Scene::Scene(window, resources, sceneManager),
	m_musicPlayer(resources),
	m_engine(std::chrono::system_clock::now().time_since_epoch().count()),
	m_distributor(0, 3)
{
	m_CONTEXT.entities = &m_entityManager;
	m_CONTEXT.inputs = &m_inputManager;
	m_CONTEXT.player = &m_musicPlayer;
}

void DemoLevel::onCreate() {
	m_CONTEXT.inputs->addInput<ba::KeyboardInput>();
	m_CONTEXT.inputs->addInput<ba::MouseInput>();

	m_entityManager.includeSystem<ba::AISystem>();
	// m_entityManager.includeSystem<ba::MovementSystem>();
	m_entityManager.includeSystem<ba::VelocityWithCollisionSystem>();
	m_entityManager.includeSystem<TS::UpdateableSystem>();
	m_entityManager.includeSystem<ba::AnimationSystem>();
	m_entityManager.includeSystem<ba::CameraSystem>();
	m_entityManager.includeSystem<ba::KeyboardControlSystem>();
	m_entityManager.includeSystem<ba::MouseControlSystem>();
	m_entityManager.includeSystem<ba::SoundSystem>();
	// m_entityManager.includeSystem<ba::CollisionSystem>();
	

	auto cs = m_entityManager.getSystem<ba::VelocityWithCollisionSystem>();
	
	cs->addCollisionLayer(PLAYER_1);
	cs->addCollisionLayer(NPC_1);
	cs->addCollisionLayer(TILE_1);
	cs->addCollisionLayer(TILE_2);
	cs->addCollisionLayer(TILE_3);
	// Tile Collision
	cs->setCollision(TILE_2, PLAYER_1);
	cs->setCollision(TILE_1, PLAYER_1);
	cs->setCollision(TILE_3, PLAYER_1);
	cs->setCollision(TILE_2, NPC_1);
	cs->setCollision(TILE_1, NPC_1);
	cs->setCollision(TILE_3, NPC_1);
	// Playe Collision
	cs->setCollision(PLAYER_1, TILE_1);
	cs->setCollision(PLAYER_1, TILE_2);
	cs->setCollision(PLAYER_1, TILE_3);
	// cs->setCollision(PLAYER_1, NPC_1);
	// NPC Collision
	cs->setCollision(NPC_1, TILE_1);
	cs->setCollision(NPC_1, TILE_2);
	cs->setCollision(NPC_1, TILE_3);
	// cs->setCollision(NPC_1, PLAYER_1);
	// cs->setCollision(NPC_1, NPC_1);
}

void DemoLevel::onDestroy() {
	m_musicPlayer.clearMusicList();
}


void DemoLevel::onActivate() {
	m_musicPlayer.addMusic("The Verdant Grove LOOP.wav");
	m_musicPlayer.setLooping(true);
	m_musicPlayer.play();

	this->generateBackground();
	this->generateMap();
	this->createHero();

	std::shared_ptr<Entity> fpsEntity = std::make_shared<Entity>(&m_CONTEXT);
	fpsEntity->setPosition({5.f, 5.f});

	m_FPSText = fpsEntity->addComponent<ba::Text>();
	m_FPSText->setDrawLayer(32);
	m_FPSText->loadFontFromFile("UbuntuMono-Bold.ttf", 16);
	m_FPSText->setColor(ba::Color::Blue);

	m_entityManager.add(fpsEntity);

	spawnBandit();
}


void DemoLevel::handleEvents() {
	m_inputManager.handleEvents();
}

void DemoLevel::update(float deltaTime) {
	m_secondsSinceLastSpawn += deltaTime;
	if (m_secondsSinceLastSpawn > 6.f) {
		spawnBandit();
		m_secondsSinceLastSpawn -= 6.f;
	}

	m_musicPlayer.update();
	int fps = static_cast<int>(std::round(1.0f / deltaTime));
	m_FPSText->setText(std::to_string(fps) + " FPS");

	m_entityManager.update(deltaTime);
}

void DemoLevel::postUpdate(float deltaTime) {
	ba::FloatRect vs = m_CONTEXT.window->getViewSpace();
	m_FPSText->getOwner()->setPosition({vs.l + 8, vs.t + 8});

	m_entityManager.postUpdate(deltaTime);
}

void DemoLevel::draw(ba::Window& window) {
	this->updateBackground(window);
	for(auto& e : m_rollingBackgrounds) {
		auto s = e->getComponent<ba::Sprite>();
		s->draw(window);
	}
	m_entityManager.draw(window);
}

void DemoLevel::updateBackground(ba::Window& window) {
	ba::FloatRect bgBounds = m_rollingBackgrounds.at(0)->getComponent<ba::Sprite>()->getGlobalBounds();
	ba::FloatRect viewSpace = window.getViewSpace();

	float x1 = static_cast<float>(static_cast<int>(viewSpace.l / bgBounds.w) * bgBounds.w);
	float y1 = viewSpace.t;
	float x2 = static_cast<float>(x1+bgBounds.w);

	for(int i = 0; i < 3; ++i) {
		m_rollingBackgrounds.at(i)->setPosition({x1, y1});
	}
	for(int i = 3; i < 6; ++i) {
		m_rollingBackgrounds.at(i)->setPosition({x2, y1});
	}
}

void DemoLevel::generateBackground() {
	const ba::Vector2f SCALE{4.f, 4.f};

	IDtype spriteBG1 = m_CONTEXT.resources->loadTexture("oak_woods_v1.0/background/background_layer_1.png");
	IDtype spriteBG2 = m_CONTEXT.resources->loadTexture("oak_woods_v1.0/background/background_layer_2.png");
	IDtype spriteBG3 = m_CONTEXT.resources->loadTexture("oak_woods_v1.0/background/background_layer_3.png");

	std::shared_ptr<Entity> bg01 = std::make_shared<Entity>(&m_CONTEXT);
	std::shared_ptr<Entity> bg02 = std::make_shared<Entity>(&m_CONTEXT);
	std::shared_ptr<Entity> bg03 = std::make_shared<Entity>(&m_CONTEXT);
	std::shared_ptr<Entity> bg11 = std::make_shared<Entity>(&m_CONTEXT);
	std::shared_ptr<Entity> bg12 = std::make_shared<Entity>(&m_CONTEXT);
	std::shared_ptr<Entity> bg13 = std::make_shared<Entity>(&m_CONTEXT);

	auto sprite01 = bg01->addComponent<ba::Sprite>();
	auto sprite02 = bg02->addComponent<ba::Sprite>();
	auto sprite03 = bg03->addComponent<ba::Sprite>();
	auto sprite11 = bg11->addComponent<ba::Sprite>();
	auto sprite12 = bg12->addComponent<ba::Sprite>();
	auto sprite13 = bg13->addComponent<ba::Sprite>();

	bg01->setScale(SCALE);
	bg02->setScale(SCALE);
	bg03->setScale(SCALE);
	bg11->setScale(SCALE);
	bg12->setScale(SCALE);
	bg13->setScale(SCALE);

	sprite01->setTexture(spriteBG1);
	sprite02->setTexture(spriteBG2);
	sprite03->setTexture(spriteBG3);
	sprite11->setTexture(spriteBG1);
	sprite12->setTexture(spriteBG2);
	sprite13->setTexture(spriteBG3);

	m_rollingBackgrounds.push_back(bg01);
	m_rollingBackgrounds.push_back(bg02);
	m_rollingBackgrounds.push_back(bg03);
	m_rollingBackgrounds.push_back(bg11);
	m_rollingBackgrounds.push_back(bg12);
	m_rollingBackgrounds.push_back(bg13);
}

void DemoLevel::generateMap() {
	const ba::Vector2f SCALE{2.f, 2.f};
	path p = m_CONTEXT.resources->getBaseDirectory() / path("Textures") / path("oak_forest.tmx");

	std::vector<std::shared_ptr<Entity>> entities = ba::generator::parseMap(p.string(), SCALE, &m_CONTEXT);
	std::vector<std::pair<int, ba::Vector2f>> objects = ba::generator::getObjects(p.string());

	m_CONTEXT.entities->add(entities);
	for (auto& pair : objects) {
		m_spawnPoints.push_back({
			pair.second.x * SCALE.x,
			pair.second.y * SCALE.y
		});
	}
}


void DemoLevel::createHero() {
	std::shared_ptr<Hero>  hero = std::make_shared<Hero>(&m_CONTEXT);

	int n = this->generateRandom();
	hero->setPosition(m_spawnPoints.at(n));

	m_entityManager.add(hero);
}

void DemoLevel::spawnBandit() {
	std::shared_ptr<Bandit> bandit = std::make_shared<Bandit>(&m_CONTEXT);

	// Choose random spawn point that is outside the current viewSpace
	ba::FloatRect vs = m_CONTEXT.window->getViewSpace();
	int rn = generateRandom();
	while(vs.contains(m_spawnPoints.at(rn))) {
		rn = generateRandom();
	}

	bandit->setPosition(m_spawnPoints.at(rn));
	m_entityManager.add(bandit);
	std::clog << "Spawned Bandit #" << bandit->ID <<" at: (" << m_spawnPoints.at(rn).x << ", " << m_spawnPoints.at(rn).y << ")\n";
}

int DemoLevel::generateRandom() {
	return m_distributor(m_engine);
}

}// namespace TS