/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fec/AbhorrantGhoulKing.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>

namespace FleshEaterCourt
{
static FactoryMethod factoryMethod = {
    AbhorrantGhoulKing::Create,
    FleshEaterCourts::ValueToString,
    FleshEaterCourts::EnumStringToInt,
    {
        {ParamType::Enum, "Grand Court", FleshEaterCourts::NoCourt, FleshEaterCourts::NoCourt, FleshEaterCourts::Gristlegore, 1},
        {ParamType::Enum, "Delusion", FleshEaterCourts::None, FleshEaterCourts::None, FleshEaterCourts::DefendersOfTheRealm, 1},
    },
    DEATH,
    { FLESH_EATER_COURTS }
};

bool AbhorrantGhoulKing::s_registered = false;

AbhorrantGhoulKing::AbhorrantGhoulKing() :
    FleshEaterCourts("Abhorrant Ghoul King", 6, WOUNDS, 10, 4, false),
    m_goryTalonsAndFangs(Weapon::Type::Melee, "Gory Talons and Fangs", 1, 6, 3, 3, -1, 1)
{
    m_keywords = {DEATH, VAMPIRE, FLESH_EATER_COURTS, ABHORRANT, HERO, WIZARD,
        ABHORRANT_GHOUL_KING};
    m_weapons = {&m_goryTalonsAndFangs};

    m_totalUnbinds = 1;
    m_totalSpells = 1;
}

bool AbhorrantGhoulKing::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_goryTalonsAndFangs);
    addModel(model);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *AbhorrantGhoulKing::Create(const ParameterList &parameters)
{
    auto unit = new AbhorrantGhoulKing();

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

void AbhorrantGhoulKing::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Abhorrant Ghoul King", factoryMethod);
    }
}

void AbhorrantGhoulKing::onStartHero(PlayerId player)
{
    // Royal Blood
    if (player == m_owningPlayer)
    {
        if (remainingWounds() < WOUNDS && remainingWounds() > 0)
        {
            Dice dice;
            int woundsHealed = dice.rollD3();
            for (auto &m : m_models)
            {
                m->applyHealing(woundsHealed);
            }
        }
    }
}

} // namespace FleshEasterCourt
