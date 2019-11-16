/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "gloomspitegitz/RippasSnarlfangs.h"

namespace GloomspiteGitz
{
static FactoryMethod factoryMethod = {
    RippasSnarlfangs::Create,
    nullptr,
    nullptr,
    {
    },
    DESTRUCTION,
    GLOOMSPITE_GITZ
};

bool RippasSnarlfangs::s_registered = false;

Unit *RippasSnarlfangs::Create(const ParameterList &parameters)
{
    auto unit = new RippasSnarlfangs();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void RippasSnarlfangs::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Rippa's Snarlfangs", factoryMethod);
    }
}

RippasSnarlfangs::RippasSnarlfangs() :
    GloomspiteGitzBase("Rippa's Snarlfangs", 12, WOUNDS, 4, 5, false),
    m_grotBow(Weapon::Type::Missile, "Grot Bow", 18, 1, 4, 4, 0, 1),
    m_bossLoppa(Weapon::Type::Melee, "Boss Loppa", 1, 2, 3, 4, -1, 1),
    m_stikka(Weapon::Type::Melee, "Stabbin' Stikka", 2, 1, 4, 4, 0, 1),
    m_bowStave(Weapon::Type::Melee, "Bow Stave", 1, 1, 5, 5, 0, 1),
    m_jaws(Weapon::Type::Melee, "Snarlfang's Jaws", 1, 2, 3, 3, 0, 2)
{
    m_keywords = {DESTRUCTION, GROT, GLOOMSPITE_GITZ, RIPPAS_SNARLFANGS};

    // Ferocious Pounce
    m_pileInMove = 6;
}

bool RippasSnarlfangs::configure()
{
    Model rippa(BASESIZE, WOUNDS);
    rippa.addMeleeWeapon(&m_bossLoppa);
    rippa.addMeleeWeapon(&m_jaws);
    rippa.setName("Rippa Narkbad");
    addModel(rippa);

    Model stabbit(BASESIZE, WOUNDS);
    stabbit.addMeleeWeapon(&m_stikka);
    stabbit.addMeleeWeapon(&m_jaws);
    stabbit.setName("Stabbit");
    addModel(stabbit);

    Model meanEye(BASESIZE, WOUNDS);
    meanEye.addMissileWeapon(&m_grotBow);
    meanEye.addMeleeWeapon(&m_bowStave);
    meanEye.addMeleeWeapon(&m_jaws);
    addModel(meanEye);

    m_points = POINTS_PER_UNIT;

    return true;
}

void RippasSnarlfangs::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_grotBow);
    visitor(m_bossLoppa);
    visitor(m_stikka);
    visitor(m_bowStave);
    visitor(m_jaws);
}

int RippasSnarlfangs::toHitModifier(const Weapon *weapon, const Unit *target) const
{
    auto mod = GloomspiteGitzBase::toHitModifier(weapon, target);

    // Smell Weakness
    // TODO: add method to unit, isWounded(), rather than loop thru the target's models here
    if (target->remainingModels() < target->initialModels())
        mod++;
    else
    {
        // Check for wounded models
        for (auto m = target->modelBegin(); m != target->modelEnd(); ++m)
        {
            if (!m->slain() && !m->fled())
            {
                if (m->woundsRemaining() < m->initialWounds())
                {
                    mod++;
                }
            }
        }
    }

    return mod;
}

} //namespace GloomspiteGitz

