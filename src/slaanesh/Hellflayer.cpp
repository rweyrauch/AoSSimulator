/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/Hellflayer.h>
#include <UnitFactory.h>
#include <Board.h>
#include "SlaaneshPrivate.h"

namespace Slaanesh {
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 7;
    static const int g_pointsPerUnit = 130;

    bool Hellflayer::s_registered = false;

    Hellflayer::Hellflayer() :
            SlaaneshBase("Hellflayer", 12, g_wounds, 10, 4, false),
            m_flensingWhips(Weapon::Type::Melee, "Flensing Whips", 2, 6, 3, 4, -1, 1),
            m_piercingClaws(Weapon::Type::Melee, "Piercing Claws", 1, 4, 3, 4, -1, 1),
            m_poisonedTongues(Weapon::Type::Melee, "Poisoned Tongues", 1, 4, 3, 4, 0, 1) {
        m_keywords = {CHAOS, DAEMON, DAEMONETTE, SLAANESH, HEDONITE, HELLFLAYER};
        m_weapons = {&m_flensingWhips, &m_piercingClaws, &m_poisonedTongues};
        m_hasMount = true;
        m_piercingClaws.setMount(true);
        m_poisonedTongues.setMount(true);
    }

    bool Hellflayer::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_flensingWhips);
        model->addMeleeWeapon(&m_piercingClaws);
        model->addMeleeWeapon(&m_poisonedTongues);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *Hellflayer::Create(const ParameterList &parameters) {
        auto unit = new Hellflayer();

        auto host = (Host) GetEnumParam("Host", parameters, g_host[0]);
        unit->setHost(host);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Hellflayer::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Hellflayer::Create,
                    SlaaneshBase::ValueToString,
                    SlaaneshBase::EnumStringToInt,
                    Hellflayer::ComputePoints,
                    {
                            EnumParameter("Host", g_host[0], g_host),
                    },
                    CHAOS,
                    {SLAANESH}
            };
            s_registered = UnitFactory::Register("Hellflayer", factoryMethod);
        }
    }

    int Hellflayer::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    int Hellflayer::extraAttacks(const Model *attackingModel, const Weapon *weapon,
                                 const Unit *target) const {
        auto extras = SlaaneshBase::extraAttacks(attackingModel, weapon, target);

        // Soulscent
        if (!weapon->isMissile()) extras += m_extraMeleeAttacks;

        return extras;
    }

    void Hellflayer::onStartCombat(PlayerId player) {
        SlaaneshBase::onStartCombat(player);

        m_extraMeleeAttacks = 0;

        // Soulscent
        if (owningPlayer() == player) {
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0f);
            for (auto u : units) {
                if (Dice::RollD6() >= 4) {
                    u->applyDamage({0, Dice::RollD3()}, this);

                    // Extra attacks
                    m_extraMeleeAttacks++;
                }
            }
        }
    }

} // Slannesh
