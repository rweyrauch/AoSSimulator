/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/TheMasque.h>
#include <UnitFactory.h>
#include <Board.h>
#include "SlaaneshPrivate.h"

namespace Slaanesh {
    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 120;

    bool TheMasque::s_registered = false;

    TheMasque::TheMasque() :
            SlaaneshBase("The Masque", 10, WOUNDS, 10, 5, false),
            m_ravagingClaws(Weapon::Type::Melee, "Ravaging Claws", 1, 6, 3, 4, -1, 1) {
        m_keywords = {CHAOS, DAEMON, DAEMONETTE, SLAANESH, HEDONITE, HERO, HERALD_OF_SLAANESH, THE_MASQUE};
        m_weapons = {&m_ravagingClaws};
        m_battleFieldRole = Leader;

        // Lithe and Swift
        m_runAndCharge = true;

        // The Endless Dance
        m_pileInMove = 6;
    }

    bool TheMasque::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_ravagingClaws);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *TheMasque::Create(const ParameterList &parameters) {
        auto unit = new TheMasque();

        auto host = (Host) GetEnumParam("Host", parameters, g_host[0]);
        unit->setHost(host);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void TheMasque::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    TheMasque::Create,
                    SlaaneshBase::ValueToString,
                    SlaaneshBase::EnumStringToInt,
                    TheMasque::ComputePoints,
                    {
                            EnumParameter("Host", g_host[0], g_host),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {SLAANESH}
            };
            s_registered = UnitFactory::Register("The Masque", factoryMethod);
        }
    }

    int TheMasque::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // Slannesh
