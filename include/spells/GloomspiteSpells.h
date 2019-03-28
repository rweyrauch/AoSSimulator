/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef GLOOMSPITESPELLS_H
#define GLOOMSPITESPELLS_H

#include <Spell.h>

enum class LoreOfTheMoonclans
{
    None,
    VindictiveGlare,
    ItchyNuisance,
    TheGreatGreenSpite,
    TheHandOfGork,
    SquigLure,
    CallDaMoon
};

std::string ToString(LoreOfTheMoonclans which);
bool FromString(const std::string& enumString, LoreOfTheMoonclans& outLore);

enum class LoreOfTheSpiderFangs
{
    None,
    DeadlyWebbing,
    VenomousSpiderlings,
    ScuttlingTerrors,
    SneakyDistraction,
    CurseOfDaSpiderGod,
    GiftOfDaSpiderGod
};

std::string ToString(LoreOfTheSpiderFangs which);
bool FromString(const std::string& enumString, LoreOfTheSpiderFangs& outLore);

Spell* CreateLoreOfTheSpiderFangs(LoreOfTheSpiderFangs which, Unit* caster);
Spell* CreateLoreOfTheMoonclans(LoreOfTheMoonclans which, Unit* caster);

DamageSpell* CreateVindictiveGlare(Unit* caster);

#endif //GLOOMSPITESPELLS_H