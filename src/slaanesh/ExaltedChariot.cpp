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
    static const int g_pointsPerUnit = 190;

    bool ExaltedChariot::s_registered = false;

    ExaltedChariot::ExaltedChariot(Host host) :
            SlaaneshBase(host, "Exalted Chariot", 10, g_wounds, 10, 4, false, g_pointsPerUnit),
            m_flensingWhips(Weapon::Type::Melee, "Flensing Whips", 2, 4, 3, 4, -1, 1),
            m_piercingClaws(Weapon::Type::Melee, "Piercing Claws", 1, 9, 3, 4, -1, 1),
            m_poisonedTongues(Weapon::Type::Melee, "Poisoned Tongues", 1, 8, 3, 4, 0, 1) {
        m_keywords = {CHAOS, DAEMON, DAEMONETTE, SLAANESH, HEDONITE, EXALTED_CHARIOT};
        m_weapons = {&m_flensingWhips, &m_piercingClaws, &m_poisonedTongues};
        m_hasMount = true;
        m_poisonedTongues.setMount(true);
        m_piercingClaws.setMount(true);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_flensingWhips);
        model->addMeleeWeapon(&m_piercingClaws);
        model->addMeleeWeapon(&m_poisonedTongues);
        addModel(model);
    }

    Unit *ExaltedChariot::Create(const ParameterList &parameters) {
        auto host = (Host) GetEnumParam("Host", parameters, g_host[0]);
        return new ExaltedChariot(host);
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
        SlaaneshBase::onCharged();

        // Excess of Blades
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0);
        for (auto ip : units) {
            int roll = Dice::RollD6();
            if (roll >= 5) {
                ip->applyDamage({0, Dice::RollD6()}, this);
            } else if (Dice::RollD6() >= 2) {
                ip->applyDamage({0, Dice::RollD3()}, this);
            }
        }
    }

    int ExaltedChariot::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void ExaltedChariot::onStartCombat(PlayerId player) {
        SlaaneshBase::onStartCombat(player);

        m_extraMeleeAttacks = 0;

        // Pungent Soulscent
        if (owningPlayer() == player) {
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0f);
            for (auto u : units) {
                if (Dice::RollD6() >= 2) {
                    u->applyDamage({0, Dice::RollD3()}, this);

                    // Extra attacks
                    m_extraMeleeAttacks++;
                }
            }
        }
    }

    int ExaltedChariot::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto extras = SlaaneshBase::extraAttacks(attackingModel, weapon, target);

        // Pungent Soulscene
        if (!weapon->isMissile()) extras += m_extraMeleeAttacks;

        return extras;
    }

} // Slannesh
