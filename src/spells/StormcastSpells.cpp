/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <spells/StormcastSpells.h>

DamageSpell* CreatePrimeElectrids(Unit* caster)
{
    return new DamageSpell(caster, "Prime Electrids", 5, 18.0f, RAND_D3, 10, RAND_D6);
}

DamageSpell* CreateSpiritStorm(Unit* caster)
{
    return new DamageSpell(caster, "Spirit Storm", 7, 18.0f, 1);
}

LineOfEffectSpell *CreatePyroelectricBlast(Unit *caster)
{
    return new LineOfEffectSpell(caster, "Pyroelectric Blast", 6, 9.0f, RAND_D3, 0);
}

AreaOfEffectSpell *CreateLightningPulse(Unit *caster)
{
    return new AreaOfEffectSpell(caster, "Lightning Pulse", 5, 0.0f, 12.0f, 1, 5);
}

AreaOfEffectSpell *CreateLightningOrb(Unit *caster)
{
    return new AreaOfEffectSpell(caster, "Lightning Orb", 6, 12.0f, 3.0f, RAND_D3, 4);
}

LineOfEffectSpell *CreateStormLance(Unit *caster)
{
    return new LineOfEffectSpell(caster, "Storm Lance", 5, 12.0f, 1, 5);
}

HealSpell *CreateHealingLight(Unit *caster)
{
    return new HealSpell(caster, "Healing Light", 5, 18.0f, RAND_D3, 10, RAND_D6);
}

Spell *CreatePurifyingBlast(Unit *caster)
{
    return nullptr;
}
