#pragma once

#include <memory>

#include "SFML/Graphics.hpp"

#include "Effect.h"
#include "GlobalEffect.h"

class EffectWrapper
{
	std::shared_ptr<Effect> effect;
	std::shared_ptr<GlobalEffect> global_effect;

public:
	EffectWrapper(Effect* _effect) : effect(_effect) {};
	EffectWrapper(GlobalEffect* _effect) : global_effect(_effect) {};

	EffectWrapper(std::shared_ptr<Effect> _effect) : effect(_effect) {};
	EffectWrapper(std::shared_ptr<GlobalEffect> _effect) : global_effect(_effect) {};

	operator std::shared_ptr<Effect>();
	operator std::shared_ptr<GlobalEffect>();

	operator Effect*();
	operator GlobalEffect*();

	bool is_global_effect();
	bool is_effect();
};

