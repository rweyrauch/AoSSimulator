/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/SyllEsske.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include <Board.h>

namespace Slaanesh
{
static FactoryMethod factoryMethod = {
    SyllEsske::Create,
    SlaaneshBase::ValueToString,
    SlaaneshBase::EnumStringToInt,
    {
        {ParamType::Enum, "Host", SlaaneshBase::Godseekers, SlaaneshBase::Invaders, SlaaneshBase::Godseekers, 1},
    },
    CHAOS,
    SLAANESH
};

bool SyllEsske::s_registered = false;

SyllEsske::SyllEsske() :
    SlaaneshBase("Syll'Esske The Vengeful Allegiance", 8, WOUNDS, 10, 4, false),
    m_axeOfDominion(Weapon::Type::Melee, "Axe of Dominion", 2, 4, 4, 3, -2, RAND_D3),
    m_scourgingWhip(Weapon::Type::Melee, "Scourging Whip", 2, 8, 3, 4, -1, 1)
{
    m_keywords = {CHAOS, DAEMON, DAEMONETTE, SLAANESH, HEDONITE, HERO, WIZARD, HERALD_OF_SLAANESH, SYLL_ESSKE, THE_VENGEFUL_ALLEGIANCE};

    // Lithe and Swift
    m_runAndCharge = true;

    m_totalSpells = 1;
    m_totalUnbinds = 1;
}

bool SyllEsske::configure()
{
    Model model(BASESIZE, WOUNDS);
    model.addMeleeWeapon(&m_axeOfDominion);
    model.addMeleeWeapon(&m_scourgingWhip);
    addModel(model);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));

    m_points = POINTS_PER_UNIT;

    return true;
}

void SyllEsske::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_axeOfDominion);
    visitor(m_scourgingWhip);
}

Unit *SyllEsske::Create(const ParameterList &parameters)
{
    auto unit = new SyllEsske();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void SyllEsske::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Syll'Esske The Vengeful Allegiance", factoryMethod);
    }
}

} // Slannesh
