/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "gloomspitegitz/RippasSnarlfangs.h"
#include "GloomspitePrivate.h"

namespace GloomspiteGitz {
    static const int g_basesize = 60; // x35 oval
    static const int g_wounds = 2;
    static const int g_pointsPerUnit = 80;

    bool RippasSnarlfangs::s_registered = false;

    Unit *RippasSnarlfangs::Create(const ParameterList &parameters) {
        auto allegiance = (Allegiance) GetEnumParam("Allegiance", parameters, g_allegiance[0]);
        return new RippasSnarlfangs(allegiance);
    }

    void RippasSnarlfangs::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    RippasSnarlfangs::Create,
                    GloomspiteGitzBase::ValueToString,
                    GloomspiteGitzBase::EnumStringToInt,
                    RippasSnarlfangs::ComputePoints,
                    {
                            EnumParameter("Allegiance", g_allegiance[0], g_allegiance),
                    },
                    DESTRUCTION,
                    {GLOOMSPITE_GITZ}
            };
            s_registered = UnitFactory::Register("Rippa's Snarlfangs", factoryMethod);
        }
    }

    RippasSnarlfangs::RippasSnarlfangs(Allegiance allegiance) :
            GloomspiteGitzBase(allegiance, "Rippa's Snarlfangs", 12, g_wounds, 4, 5, false, g_pointsPerUnit),
            m_grotBow(Weapon::Type::Missile, "Grot Bow", 18, 1, 4, 4, 0, 1),
            m_bossLoppa(Weapon::Type::Melee, "Boss Loppa", 1, 2, 3, 4, -1, 1),
            m_stikka(Weapon::Type::Melee, "Stabbin' Stikka", 2, 1, 4, 4, 0, 1),
            m_bowStave(Weapon::Type::Melee, "Bow Stave", 1, 1, 5, 5, 0, 1),
            m_jaws(Weapon::Type::Melee, "Snarlfang's Jaws", 1, 2, 3, 3, 0, 2) {
        m_keywords = {DESTRUCTION, GROT, GLOOMSPITE_GITZ, RIPPAS_SNARLFANGS};
        m_weapons = {&m_grotBow, &m_bossLoppa, &m_stikka, &m_bowStave, &m_jaws};

        // Ferocious Pounce
        m_pileInMove = 6;

        auto rippa = new Model(g_basesize, wounds());
        rippa->addMeleeWeapon(&m_bossLoppa);
        rippa->addMeleeWeapon(&m_jaws);
        rippa->setName("Rippa Narkbad");
        addModel(rippa);

        auto stabbit = new Model(g_basesize, wounds());
        stabbit->addMeleeWeapon(&m_stikka);
        stabbit->addMeleeWeapon(&m_jaws);
        stabbit->setName("Stabbit");
        addModel(stabbit);

        auto meanEye = new Model(g_basesize, wounds());
        meanEye->addMissileWeapon(&m_grotBow);
        meanEye->addMeleeWeapon(&m_bowStave);
        meanEye->addMeleeWeapon(&m_jaws);
        addModel(meanEye);
    }

    int RippasSnarlfangs::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = GloomspiteGitzBase::toHitModifier(weapon, target);

        // Smell Weakness
        if (target->numOfWoundedModels() > 0) {
            mod++;
        }

        return mod;
    }

    int RippasSnarlfangs::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} //namespace GloomspiteGitz

