/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/LordOfPain.h>
#include <UnitFactory.h>
#include <Board.h>
#include "SlaaneshPrivate.h"

namespace Slaanesh {
    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 120;

    bool LordOfPain::s_registered = false;

    LordOfPain::LordOfPain() :
            SlaaneshBase("Lord of Pain", 6, g_wounds, 7, 4, false),
            m_mace(Weapon::Type::Melee, "Soulpiercer Mace", 2, 5, 3, 3, -1, 2) {
        m_keywords = {CHAOS, MORTAL, SLAANESH, HEDONITE, HERO, LORD_OF_PAIN};
        m_weapons = {&m_mace};
        m_battleFieldRole = Leader;
    }

    bool LordOfPain::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_mace);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *LordOfPain::Create(const ParameterList &parameters) {
        auto unit = new LordOfPain();

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

    void LordOfPain::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    LordOfPain::Create,
                    SlaaneshBase::ValueToString,
                    SlaaneshBase::EnumStringToInt,
                    LordOfPain::ComputePoints,
                    {
                            EnumParameter("Host", g_host[0], g_host),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {SLAANESH}
            };
            s_registered = UnitFactory::Register("Lord of Pain", factoryMethod);
        }
    }

    int LordOfPain::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // Slannesh
