/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fec/AbhorrantArchregent.h>
#include <UnitFactory.h>
#include <Spell.h>
#include <spells/MysticShield.h>

namespace FleshEaterCourt
{
static FactoryMethod factoryMethod = {
    AbhorrantArchregent::Create,
    FleshEaterCourts::ValueToString,
    FleshEaterCourts::EnumStringToInt,
    AbhorrantArchregent::ComputePoints,
    {
        {ParamType::Enum, "Grand Court", FleshEaterCourts::NoCourt, FleshEaterCourts::NoCourt, FleshEaterCourts::Gristlegore, 1},
        {ParamType::Enum, "Delusion", FleshEaterCourts::None, FleshEaterCourts::None, FleshEaterCourts::DefendersOfTheRealm, 1},
    },
    DEATH,
    { FLESH_EATER_COURTS }
};

bool AbhorrantArchregent::s_registered = false;

AbhorrantArchregent::AbhorrantArchregent() :
    FleshEaterCourts("Abhorrant Archregent", 6, WOUNDS, 10, 4, false),
    m_goryTalonsAndFangs(Weapon::Type::Melee, "Gory Talons and Fangs", 1, 7, 3, 3, -1, 1)
{
    m_keywords = {DEATH, VAMPIRE, FLESH_EATER_COURTS, ABHORRANT, HERO, WIZARD, ABHORRANT_ARCHREGENT};
    m_weapons = {&m_goryTalonsAndFangs};

    m_totalUnbinds = 2;
    m_totalSpells = 2;
}

bool AbhorrantArchregent::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_goryTalonsAndFangs);
    addModel(model);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *AbhorrantArchregent::Create(const ParameterList &parameters)
{
    auto unit = new AbhorrantArchregent();

    auto court = (GrandCourt)GetEnumParam("Grand Court", parameters, NoCourt);
    auto delusion = (Delusion)GetEnumParam("Delusion", parameters, None);
    // TODO: error checks - can only select delusion if GrandCourt is NoCourt.
    unit->setGrandCourt(court);
    unit->setCourtsOfDelusion(delusion);

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void AbhorrantArchregent::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Abhorrant Archregent", factoryMethod);
    }
}

void AbhorrantArchregent::onStartHero(PlayerId player)
{
    // Imperial Blood
    if (player == m_owningPlayer)
    {
        if (remainingWounds() < WOUNDS && remainingWounds() > 0)
        {
            int woundsHealed = 3;
            for (auto &m : m_models)
            {
                m->applyHealing(woundsHealed);
            }
        }
    }

}

} // namespace FleshEasterCourt
