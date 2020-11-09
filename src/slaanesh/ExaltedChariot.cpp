/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/ExaltedChariot.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include <Board.h>
#include "SlaaneshPrivate.h"

namespace Slaanesh {
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 9;
    static const int g_pointsPerUnit = 180;

    bool ExaltedChariot::s_registered = false;

    ExaltedChariot::ExaltedChariot() :
            SlaaneshBase("Exalted Chariot", 10, g_wounds, 10, 4, false),
            m_flensingWhips(Weapon::Type::Melee, "Flensing Whips", 2, 4, 3, 4, -1, 1),
            m_piercingClaws(Weapon::Type::Melee, "Piercing Claws", 1, 9, 3, 4, -1, 1),
            m_poisonedTongues(Weapon::Type::Melee, "Poisoned Tongues", 1, 8, 3, 4, 0, 1) {
        m_keywords = {CHAOS, DAEMON, DAEMONETTE, SLAANESH, HEDONITE, EXALTED_CHARIOT};
        m_weapons = {&m_flensingWhips, &m_piercingClaws, &m_poisonedTongues};
        m_hasMount = true;
    }

    bool ExaltedChariot::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_flensingWhips);
        model->addMeleeWeapon(&m_piercingClaws);
        model->addMeleeWeapon(&m_poisonedTongues);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *ExaltedChariot::Create(const ParameterList &parameters) {
        auto unit = new ExaltedChariot();

        auto host = (Host) GetEnumParam("Host", parameters, g_host[0]);
        unit->setHost(host);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void ExaltedChariot::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    ExaltedChariot::Create,
                    SlaaneshBase::ValueToString,
                    SlaaneshBase::EnumStringToInt,
                    ExaltedChariot::ComputePoints,
                    {
                            EnumParameter("Host", g_host[0], g_host),
                    },
                    CHAOS,
                    {SLAANESH}
            };
            s_registered = UnitFactory::Register("Exalted Chariot", factoryMethod);
        }
    }

    void ExaltedChariot::onCharged() {
        Unit::onCharged();

        // Excess of Blades
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0);
        for (auto ip : units) {
            int roll = Dice::RollD6();
            if (roll >= 5) {
                ip->applyDamage({0, Dice::RollD6()});
            } else if (Dice::RollD6() >= 2) {
                ip->applyDamage({0, Dice::RollD3()});
            }
        }
    }

    int ExaltedChariot::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // Slannesh
