/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef STORMCASTSPELLS_H
#define STORMCASTSPELLS_H

#include <Spell.h>

DamageSpell* CreatePrimeElectrids(Unit* caster);
DamageSpell* CreateSpiritStorm(Unit* caster);
LineOfEffectSpell* CreatePyroelectricBlast(Unit* caster);
AreaOfEffectSpell* CreateLightningPulse(Unit* caster);
AreaOfEffectSpell* CreateLightningOrb(Unit* caster);
LineOfEffectSpell* CreateStormLance(Unit* caster);
HealSpell* CreateHealingLight(Unit* caster);
Spell* CreatePurifyingBlast(Unit* caster);
Spell* CreateThunderclap(Unit* caster);

#endif //STORMCASTSPELLS_H