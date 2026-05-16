#include "EffectWrapper.h"

EffectWrapper::operator std::shared_ptr<Effect>()
{
    return effect;
}

EffectWrapper::operator std::shared_ptr<GlobalEffect>()
{
    return global_effect;
}

EffectWrapper::operator Effect* ()
{
    return effect.get();
}

EffectWrapper::operator GlobalEffect* ()
{
    return global_effect.get();
}

bool EffectWrapper::is_global_effect()
{
    return global_effect.operator bool();
}

bool EffectWrapper::is_effect()
{
    return effect.operator bool();
}
