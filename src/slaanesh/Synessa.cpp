/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/Synessa.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include <slaanesh/Lore.h>
#include "SlaaneshPrivate.h"

namespace Slaanesh {
    static const int g_basesize = 105; // x70 oval
    static const int g_wounds = 9;
    static const int g_pointsPerUnit = 260;

    bool Synessa::s_registered = false;

    Synessa::Synessa(Host host, Lore lore, bool isGeneral) :
            SlaaneshBase(host, "Synessa", 8, g_wounds, 10, 4, false, g_pointsPerUnit) {
        m_keywords = {CHAOS, DAEMON, SLAANESH, HEDONITE, HERO, WIZARD, MONSTER, SYNESSA};
        m_weapons = {&m_staff, &m_talons};
        m_battleFieldRole = Role::Leader;

        m_totalSpells = 1;
        m_totalUnbinds = 1;

        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_staff);
        model->addMeleeWeapon(&m_talons);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    }

    Unit *Synessa::Create(const ParameterList &parameters) {
        auto host = (Host) GetEnumParam("Host", parameters, g_host[0]);
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_daemonLore[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new Synessa(host, lore, general);
    }

    void Synessa::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Synessa::Create,
                    SlaaneshBase::ValueToString,
                    SlaaneshBase::EnumStringToInt,
                    Synessa::ComputePoints,
                    {
                            EnumParameter("Host", g_host[0], g_host),
                            EnumParameter("Lore", g_daemonLore[0], g_daemonLore),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {SLAANESH}
            };
            s_registered = UnitFactory::Register("Synessa", factoryMethod);
        }
    }

    int Synessa::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // Slannesh
