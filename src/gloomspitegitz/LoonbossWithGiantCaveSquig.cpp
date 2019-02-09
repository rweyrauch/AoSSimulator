/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <gloomspitegitz/LoonbossWithGiantCaveSquig.h>
#include <UnitFactory.h>
#include <Board.h>
#include <Roster.h>

namespace GloomspiteGitz
{
static FactoryMethod factoryMethod = {
    LoonbossWithGiantCaveSquig::Create,
    nullptr,
    nullptr,
    {
    }
};

bool LoonbossWithGiantCaveSquig::s_registered = false;

LoonbossWithGiantCaveSquig::LoonbossWithGiantCaveSquig() :
    Unit("Loonboss with Giant Cave Squig", RAND_2D6, WOUNDS, 6, 4, true),
    m_moonProdderMissile(Weapon::Type::Missile, "Moon-prodder", 14, RAND_D6, 4, 3, -1, 1),
    m_moonProdder(Weapon::Type::Melee, "Moon-prodder", 2, 4, 4, 3, -1, 1),
    m_massiveFangFilledGob(Weapon::Type::Melee, "Massive Fang-filled Gob", 1, 4, 4, 3, -1, RAND_D3)
{
    m_keywords = {DESTRUCTION, GROT, GLOOMSPITE_GITZ, MOONCLAN, HERO, LOONBOSS};
}

bool LoonbossWithGiantCaveSquig::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMissileWeapon(&m_moonProdderMissile);
    model.addMeleeWeapon(&m_moonProdder);
    model.addMeleeWeapon(&m_massiveFangFilledGob);

    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void LoonbossWithGiantCaveSquig::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_moonProdderMissile);
    visitor(&m_moonProdder);
    visitor(&m_massiveFangFilledGob);
}

Unit *LoonbossWithGiantCaveSquig::Create(const ParameterList &parameters)
{
    auto unit = new LoonbossWithGiantCaveSquig();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void LoonbossWithGiantCaveSquig::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Loonboss with Giant Cave Squig", factoryMethod);
    }
}

int LoonbossWithGiantCaveSquig::targetHitModifier(const Weapon *weapon, const Unit *attacker) const
{
    int modifier = Unit::targetHitModifier(weapon, attacker);
    // Dead Tricksy
    modifier -= 1;
    return modifier;
}

} // namespace GloomspiteGitz

