/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "skaven/SpiteclawsSwarm.h"

namespace Skaven {
    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int POINTS_PER_UNIT = 70;

    bool SpiteclawsSwarm::s_registered = false;

    Unit *SpiteclawsSwarm::Create(const ParameterList &parameters) {
        auto unit = new SpiteclawsSwarm();

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    int SpiteclawsSwarm::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

    void SpiteclawsSwarm::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    Skaventide::ValueToString,
                    Skaventide::EnumStringToInt,
                    ComputePoints,
                    {
                    },
                    CHAOS,
                    {SKAVEN}
            };

            s_registered = UnitFactory::Register("Spiteclaw's Swarm", factoryMethod);
        }
    }

    SpiteclawsSwarm::SpiteclawsSwarm() :
            Skaventide("Spiteclaw's Swarm", 6, WOUNDS, 4, 6, false),
            m_stabbingBlade(Weapon::Type::Melee, "Festering Skaven's Stabbing Blades", 1, 2, 4, 4, 0, 1),
            m_rustyFlail(Weapon::Type::Melee, "Hungering Skaven's Rusty Flail", 1, 1, 4, 4, 0, 1),
            m_rustySpear(Weapon::Type::Melee, "Krrk's Rusty Spear", 2, 2, 4, 4, 0, 1),
            m_punchDaggers(Weapon::Type::Melee, "Lurking Skaven's Punch Daggers", 1, 2, 4, 4, 0, 1) {
        m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, CLANS_VERMINUS, SPITECLAWS_SWARM};
        m_weapons = {&m_stabbingBlade, &m_rustyFlail, &m_rustySpear, &m_punchDaggers};
    }

    bool SpiteclawsSwarm::configure() {
        auto krrk = new Model(BASESIZE, wounds());
        krrk->addMeleeWeapon(&m_rustySpear);
        krrk->setName("Krrk the Almost-trusted");
        addModel(krrk);

        auto rat1 = new Model(BASESIZE, wounds());
        rat1->addMeleeWeapon(&m_rustyFlail);
        addModel(rat1);

        auto rat2 = new Model(BASESIZE, wounds());
        rat2->addMeleeWeapon(&m_stabbingBlade);
        addModel(rat2);

        auto rat3 = new Model(BASESIZE, wounds());
        rat3->addMeleeWeapon(&m_punchDaggers);
        addModel(rat3);

        m_points = POINTS_PER_UNIT;

        return true;
    }

} // namespace Skaven
