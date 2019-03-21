/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <greenskinz/OrrukGreatShaman.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Greenskinz
{
static FactoryMethod factoryMethod = {
    OrrukGreatShaman::Create,
    nullptr,
    nullptr,
    {
        {ParamType::Boolean, "War Boar", {.m_boolValue = false}, false, false, false},
    },
    DESTRUCTION,
    GREENSKINZ
};

bool OrrukGreatShaman::s_registered = false;

OrrukGreatShaman::OrrukGreatShaman() :
    Unit("Orruk Great Shaman", 5, WOUNDS, 6, 5, false),
    m_totemicStaff(Weapon::Type::Melee, "Totemic Staff", 2, 1, 4, 3, 0, RAND_D3),
    m_boarsTusks(Weapon::Type::Melee, "War Boar's Tusks", 1, 2, 4, 4, 0, 1)
{
    m_keywords = {DESTRUCTION, ORRUK, GREENSKINZ, HERO, WIZARD, ORRUK_GREAT_SHAMAN};
}

bool OrrukGreatShaman::configure(bool warboar)
{
    Model model(BASESIZE, WOUNDS);
    model.addMeleeWeapon(&m_totemicStaff);
    if (warboar)
    {
        model.addMeleeWeapon(&m_boarsTusks);
        m_move = 9;
    }
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void OrrukGreatShaman::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_totemicStaff);
    visitor(&m_boarsTusks);
}

Unit *OrrukGreatShaman::Create(const ParameterList &parameters)
{
    auto unit = new OrrukGreatShaman();
    bool warboar = GetBoolParam("War Boar", parameters, false);

    bool ok = unit->configure(warboar);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void OrrukGreatShaman::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Orruk Great Shaman", factoryMethod);
    }
}

int OrrukGreatShaman::castingModifier() const
{
    auto modifier = Unit::castingModifier();

    // Waaagh! Energy
    auto units = Board::Instance()->getUnitsWithin(this, m_owningPlayer, 8.0f);
    int totalOrruks = 0;
    for (auto ip : units)
    {
        if (ip->hasKeyword(ORRUK))
        {
            totalOrruks == ip->remainingModels();
        }
    }
    if (totalOrruks >= 20)
    {
        modifier += 1;
    }
    return modifier;
}

} // namespace Greenskinz
