#include "TS/Entities/SlideDust.hpp"

namespace TS {

const ba::IDtype SLIDE_DUST_EFFECT = 1u;

bool SlideDust::s_resourcesLoaded = false;

const std::unordered_map<ba::IDtype, std::pair<float, std::vector<std::string>>> SlideDust::s_gfxToLoad{
	{SLIDE_DUST_EFFECT,
		{0.75f,
			{
				"Effects/SlideDust/SlideDust_0.png",
				"Effects/SlideDust/SlideDust_1.png",
				"Effects/SlideDust/SlideDust_2.png",
				"Effects/SlideDust/SlideDust_3.png",
				"Effects/SlideDust/SlideDust_4.png",
			}
		}
	}
};

std::unordered_map<ba::IDtype, std::pair<float, std::vector<ba::IDtype>>> SlideDust::s_FXR{};

const ba::FloatRect g_SD_RECT{
	0.F, 0.F, 25.F, 32.F,
};

const ba::Vector2f g_SD_SCALE{
	2.F, 2.F
};

SlideDust::SlideDust(ba::SharedContext* context) :
	ba::Entity::Entity(context)
{
	loadResources();
	
	this->addComponent<ba::Animation>();
	auto sprite = this->addComponent<ba::Sprite>();

	this->setScale(g_SD_SCALE);
	this->setOrigin({g_SD_RECT.w / 2.f, g_SD_RECT.h / 2.f});

	populateAnimation();

	sprite->setDrawLayer(32u);
}

void SlideDust::loadResources() {
	if (s_resourcesLoaded) {
		return;
	}
	for (auto& [id, pair] : s_gfxToLoad) {
		if (!s_FXR.contains(id)) {
			s_FXR.insert_or_assign(id, std::make_pair(pair.first, std::vector<ba::IDtype>{}));
		}
		for (auto& str : pair.second) {	
			s_FXR.at(id).second.push_back(this->CONTEXT->resources->loadTexture(str));
		}
	}

	s_resourcesLoaded = true;
}

void SlideDust::populateAnimation() {
	auto animation = this->getComponent<ba::Animation>();
	for (auto& [animationID, pair] : s_FXR) {
		const float SECONDS_PER_FRAME = pair.first / pair.second.size();
		ba::Sequence s;
		for (const auto& RES_ID : pair.second) {
			s.frames.push_back(ba::Frame{
				RES_ID,
				g_SD_RECT,
				SECONDS_PER_FRAME
			});
		}
		
		animation->add(animationID, s);
	}

	animation->addSequenceAction(SLIDE_DUST_EFFECT, std::bind([this]() {
		this->queueForRemoval(true);
	}));

	animation->set(SLIDE_DUST_EFFECT);
}



}// namespace TS