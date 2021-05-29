/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/Dexcessa.h>
#include <UnitFactory.h>
#include "SlaaneshPrivate.h"

namespace Slaanesh {
    static const int g_basesize = 105; // x70 oval
    static const int g_wounds = 10;
    static const int g_pointsPerUnit = 280;

    bool Dexcessa::s_registered = false;

    Dexcessa::Dexcessa(Host host, bool isGeneral) :
            SlaaneshBase(host, "Dexcessa", 12, g_wounds, 10, 4, true, g_pointsPerUnit) {
        m_keywords = {CHAOS, DAEMON, SLAANESH, HEDONITE, HERO, MONSTER, DEXCESSA};
        m_weapons = {&m_scourge, &m_talons};
        m_battleFieldRole = Role::Leader;

        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_scourge);
        model->addMeleeWeapon(&m_talons);
        addModel(model);
    }

    Unit *Dexcessa::Create(const ParameterList &parameters) {
        auto host = (Host) GetEnumParam("Host", parameters, g_host[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new Dexcessa(host, general);
    }

    void Dexcessa::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Dexcessa::Create,
                    SlaaneshBase::ValueToString,
                    SlaaneshBase::EnumStringToInt,
                    Dexcessa::ComputePoints,
                    {
                            EnumParameter("Host", g_host[0], g_host),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {SLAANESH}
            };
            s_registered = UnitFactory::Register("Dexcessa", factoryMethod);
        }
    }

    int Dexcessa::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // Slannesh
