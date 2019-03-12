/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fec/AbhorrantArchregent.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include <spells/ArcaneBolt.h>

namespace FleshEaterCourt
{
static FactoryMethod factoryMethod = {
    AbhorrantArchregent::Create,
    nullptr,
    nullptr,
    {
    },
    DEATH,
    FLESH_EATER_COURTS
};

bool AbhorrantArchregent::s_registered = false;

AbhorrantArchregent::AbhorrantArchregent() :
    FleshEaterCourts("Abhorrant Archregent", 6, WOUNDS, 10, 4, false),
    m_goryTalonsAndFangs(Weapon::Type::Melee, "Gory Talons and Fangs", 1, 7, 3, 3, -1, 1)
{
    m_keywords = {DEATH, VAMPIRE, FLESH_EATER_COURTS, ABHORRANT, HERO, WIZARD,
        ABHORRANT_ARCHREGENT};

    m_totalUnbinds = 2;
    m_totalSpells = 2;
}

bool AbhorrantArchregent::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMeleeWeapon(&m_goryTalonsAndFangs);
    addModel(model);

    m_knownSpells.push_back(std::make_unique<ArcaneBolt>(this));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));

    m_points = POINTS_PER_UNIT;

    return true;
}

void AbhorrantArchregent::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_goryTalonsAndFangs);
}

Unit *AbhorrantArchregent::Create(const ParameterList &parameters)
{
    auto unit = new AbhorrantArchregent();

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
                m.woundsRemaining() += woundsHealed;
                if (m.woundsRemaining() > WOUNDS)
                {
                    m.woundsRemaining() = WOUNDS;
                }
            }
        }
    }

}

} // namespace FleshEasterCourt
