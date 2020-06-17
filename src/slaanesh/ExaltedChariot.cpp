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
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 9;
    static const int POINTS_PER_UNIT = 180;

    bool ExaltedChariot::s_registered = false;

    ExaltedChariot::ExaltedChariot() :
            SlaaneshBase("Exalted Chariot", 10, WOUNDS, 10, 4, false),
            m_flensingWhips(Weapon::Type::Melee, "Flensing Whips", 2, 4, 3, 4, -1, 1),
            m_piercingClaws(Weapon::Type::Melee, "Piercing Claws", 1, 9, 3, 4, -1, 1),
            m_poisonedTongues(Weapon::Type::Melee, "Poisoned Tongues", 1, 8, 3, 4, 0, 1) {
        m_keywords = {CHAOS, DAEMON, DAEMONETTE, SLAANESH, HEDONITE, EXALTED_CHARIOT};
        m_weapons = {&m_flensingWhips, &m_piercingClaws, &m_poisonedTongues};
    }

    bool ExaltedChariot::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_flensingWhips);
        model->addMeleeWeapon(&m_piercingClaws);
        model->addMeleeWeapon(&m_poisonedTongues);
        addModel(model);

        m_points = POINTS_PER_UNIT;

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
            int roll = Dice::rollD6();
            if (roll >= 5) {
                ip->applyDamage({0, Dice::rollD6()});
            } else if (Dice::rollD6() >= 2) {
                ip->applyDamage({0, Dice::rollD3()});
            }
        }
    }

    int ExaltedChariot::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // Slannesh
