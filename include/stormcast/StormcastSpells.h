/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <Spell.h>

namespace StormcastEternals {

    DamageSpell *CreatePrimeElectrids(Unit *caster);

    DamageSpell *CreateSpiritStorm(Unit *caster);

    LineOfEffectSpell *CreatePyroelectricBlast(Unit *caster);

    AreaOfEffectSpell *CreateLightningPulse(Unit *caster);

    AreaOfEffectSpell *CreateLightningOrb(Unit *caster);

    LineOfEffectSpell *CreateStormLance(Unit *caster);

    HealSpell *CreateHealingLight(Unit *caster);

    Spell *CreatePurifyingBlast(Unit *caster);

    Spell *CreateThunderclap(Unit *caster);

    Spell *CreateStormsire(Unit *caster);
}