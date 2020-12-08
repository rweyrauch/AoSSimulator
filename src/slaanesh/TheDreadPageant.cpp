/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/TheDreadPageant.h>
#include <UnitFactory.h>
#include <Board.h>
#include "SlaaneshPrivate.h"

namespace Slaanesh {
    static const int g_basesize = 32;
    static const int g_wounds = 1;
    static const int g_pointsPerUnit = 120;

    bool TheDreadPageant::s_registered = false;

    TheDreadPageant::TheDreadPageant() :
            SlaaneshBase("The Dread Pageant", 6, g_wounds, 7, 5, false),
            m_bow(Weapon::Type::Missile, "Blissbarb Bow", 18, 2, 4, 4, -1, 1),
            m_clawAndWeapon(Weapon::Type::Melee, "Crushing Claw and Gilded Weapon", 1, 4, 4, 3, -2, 2),
            m_glaive(Weapon::Type::Melee, "Slickblade Glaive", 1, 3, 3, 3, -1, 1),
            m_harpoon(Weapon::Type::Melee, "Dread Harpoon", 1, 4, 2, 3, -1, RAND_D3) {
        m_keywords = {CHAOS, MORTAL, SLAANESH, HEDONITE, DREAD_PAGEANT};
        m_weapons = {&m_bow, &m_clawAndWeapon, &m_glaive, &m_harpoon};
    }

    bool TheDreadPageant::configure() {
        auto vasillac = new Model(g_basesize, wounds()+2);
        vasillac->addMeleeWeapon(&m_harpoon);
        addModel(vasillac);

        auto slakeslash = new Model(g_basesize, wounds()+2);
        slakeslash->addMeleeWeapon(&m_clawAndWeapon);
        addModel(slakeslash);

        auto glissete = new Model(g_basesize, wounds());
        glissete->addMeleeWeapon(&m_glaive);
        addModel(glissete);

        auto hadzu = new Model(g_basesize, wounds());
        hadzu->addMissileWeapon(&m_bow);
        addModel(hadzu);

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *TheDreadPageant::Create(const ParameterList &parameters) {
        auto unit = new TheDreadPageant();

        auto host = (Host) GetEnumParam("Host", parameters, g_host[0]);
        unit->setHost(host);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void TheDreadPageant::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    TheDreadPageant::Create,
                    SlaaneshBase::ValueToString,
                    SlaaneshBase::EnumStringToInt,
                    TheDreadPageant::ComputePoints,
                    {
                            EnumParameter("Host", g_host[0], g_host)
                    },
                    CHAOS,
                    {SLAANESH}
            };
            s_registered = UnitFactory::Register("The Dread Pageant", factoryMethod);
        }
    }

    int TheDreadPageant::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // Slannesh
