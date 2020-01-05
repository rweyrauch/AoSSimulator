/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <eldritchcouncil/Loremaster.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>

namespace EldritchCouncil
{
static FactoryMethod factoryMethod = {
    Loremaster::Create,
    nullptr,
    nullptr,
    {
    },
    ORDER,
    ELDRITCH_COUNCIL
};

bool Loremaster::s_registered = false;

Loremaster::Loremaster() :
    Unit("Loremaster", 6, WOUNDS, 7, 4, false),
    m_greatsword(Weapon::Type::Melee, "Greatsword", 1, 2, 3, 3, -1, 1)
{
    m_keywords = {ORDER, AELF, ELDRITCH_COUNCIL, HERO, WIZARD, LOREMASTER};
    m_weapons = {&m_greatsword};

    m_totalUnbinds = 1;
    m_totalSpells = 1;
}

bool Loremaster::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_greatsword);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));

    return true;
}

Unit *Loremaster::Create(const ParameterList &parameters)
{
    auto unit = new Loremaster();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Loremaster::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Loremaster", factoryMethod);
    }
}

Rerolls Loremaster::toSaveRerolls(const Weapon *weapon) const
{
    // Deflect Shots
    if (weapon->isMissile())
    {
        return RerollFailed;
    }
    return Unit::toSaveRerolls(weapon);
}

} //namespace EldritchCouncil