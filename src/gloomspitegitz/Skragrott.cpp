/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <gloomspitegitz/Skragrott.h>
#include <UnitFactory.h>
#include <iostream>
#include <Board.h>
#include <spells/MysticShield.h>
#include <spells/NikkitNikkit.h>

namespace GloomspiteGitz
{
static FactoryMethod factoryMethod = {
    Skragrott::Create,
    Skragrott::ValueToString,
    Skragrott::EnumStringToInt,
    {
        {ParamType::Enum, "Lore of the Moonclans", (int)LoreOfTheMoonclans::None, (int)LoreOfTheMoonclans::None, (int)LoreOfTheMoonclans::CallDaMoon, 1},
    },
    DESTRUCTION,
    GLOOMSPITE_GITZ
};

bool Skragrott::s_registered = false;

Skragrott::Skragrott() :
    GloomspiteGitzBase("Skragrott", 4, WOUNDS, 6, 5, false),
    m_daMoonOnnaStikkMissile(Weapon::Type::Missile, "Puff Spores", 8, 1, 5, 5, 0, RAND_D3),
    m_daMoonOnnaStikk(Weapon::Type::Melee, "Enormous Jaws", 3, 8, 2, 3, -2, RAND_D3)
{
    m_keywords = {DESTRUCTION, GROT, GLOOMSPITE_GITZ, MOONCLAN, HERO, WIZARD, LOONBOSS, SKRAGROTT};

    m_totalUnbinds = 2;
    m_totalSpells = 2;
}

bool Skragrott::configure(LoreOfTheMoonclans lore)
{
        auto model = new Model(BASESIZE, WOUNDS);

    model->addMissileWeapon(&m_daMoonOnnaStikkMissile);
    model->addMeleeWeapon(&m_daMoonOnnaStikk);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    m_knownSpells.push_back(std::make_unique<NikkitNikkit>(this));
    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLoreOfTheMoonclans(lore, this)));

    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *Skragrott::Create(const ParameterList &parameters)
{
    auto unit = new Skragrott();
    auto lore = (LoreOfTheMoonclans)GetEnumParam("Lore of the Moonclans", parameters, (int)LoreOfTheMoonclans ::None);

    bool ok = unit->configure(lore);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Skragrott::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Skragrott", factoryMethod);
    }
}

void Skragrott::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_daMoonOnnaStikkMissile);
    visitor(m_daMoonOnnaStikk);
}

std::string Skragrott::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Lore of the Moonclans")
    {
        return ToString((LoreOfTheMoonclans)parameter.m_intValue);
    }
    return ParameterValueToString(parameter);
}

int Skragrott::EnumStringToInt(const std::string &enumString)
{
    LoreOfTheMoonclans lore;
    if (FromString(enumString, lore))
    {
        return (int) lore;
    }
    return 0;
}

int Skragrott::castingModifier() const
{
    // Loonking's Crown
    return GloomspiteGitzBase::castingModifier() + 1;
}

int Skragrott::unbindingModifier() const
{
    // Loonking's Crown
    return Unit::unbindingModifier() + 1;
}

Wounds Skragrott::applyWoundSave(const Wounds &wounds)
{
    // Loonking's Crown
    Dice dice;
    Dice::RollResult result;

    Dice::RollResult woundSaves, mortalSaves;
    dice.rollD6(wounds.normal, woundSaves);
    dice.rollD6(wounds.mortal, mortalSaves);

    Wounds totalWounds = wounds;
    totalWounds.normal -= woundSaves.rollsGE(4);
    totalWounds.normal = std::max(totalWounds.normal, 0);
    totalWounds.mortal -= mortalSaves.rollsGE(4);
    totalWounds.mortal = std::max(totalWounds.mortal, 0);

    return totalWounds;
}

} // namespace GloomspiteGitz
