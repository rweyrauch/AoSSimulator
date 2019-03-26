/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LOREOFTHESTORM_H
#define LOREOFTHESTORM_H

#include <Spell.h>

DamageSpell* CreateLightningBlast(Unit* caster);
AreaOfEffectSpell* CreateStarfall(Unit* caster);
AreaOfEffectSpell* CreateThundershock(Unit* caster);
Spell* CreateAzyriteHalo(Unit* caster);
AreaOfEffectSpell* CreateChainLightning(Unit* caster);
AreaOfEffectSpell* CreateStormcaller(Unit* caster);

Spell* CreateTerrifyingAspect(Unit* caster);
Spell* CreateCelestialBlades(Unit* caster);
Spell* CreateSpeedOfLightning(Unit* caster);

#endif //LOREOFTHESTORM_H