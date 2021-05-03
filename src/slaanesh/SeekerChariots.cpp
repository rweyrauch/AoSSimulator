/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/SeekerChariots.h>
#include <UnitFactory.h>
#include <Board.h>
#include "SlaaneshPrivate.h"

namespace Slaanesh {
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 6;
    static const int g_minUnitSize = 1;
    static const int g_maxUnitSize = 3;
    static const int g_pointsPerBlock = 130;
    static const int g_pointsMaxUnitSize = (g_maxUnitSize / g_minUnitSize) * g_pointsPerBlock;

    bool SeekerChariots::s_registered = false;

    SeekerChariots::SeekerChariots(Host host, int numModels, int points) :
            SlaaneshBase(host, "Seeker Chariots", 12, g_wounds, 10, 4, false, points),
            m_flensingWhips(Weapon::Type::Melee, "Flensing Whips", 2, 4, 3, 4, -1, 1),
            m_piercingClaws(Weapon::Type::Melee, "Piercing Claws", 1, 3, 3, 4, -1, 1),
            m_poisonedTongues(Weapon::Type::Melee, "Poisoned Tongues", 1, 4, 3, 4, 0, 1) {
        m_keywords = {CHAOS, DAEMON, DAEMONETTE, SLAANESH, HEDONITE, SEEKER_CHARIOTS};
        m_weapons = {&m_flensingWhips, &m_piercingClaws, &m_poisonedTongues};
        m_hasMount = true;
        m_poisonedTongues.setMount(true);
        m_piercingClaws.setMount(true);
        // Impossibly Swift
        m_retreatAndCharge = true;

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_flensingWhips);
            model->addMeleeWeapon(&m_piercingClaws);
            model->addMeleeWeapon(&m_poisonedTongues);
            addModel(model);
        }
    }

    Unit *SeekerChariots::Create(const ParameterList &parameters) {
        auto host = (Host) GetEnumParam("Host", parameters, g_host[0]);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        return new SeekerChariots(host, numModels, ComputePoints(parameters));
    }

    void SeekerChariots::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    SeekerChariots::Create,
                    SlaaneshBase::ValueToString,
                    SlaaneshBase::EnumStringToInt,
                    SeekerChariots::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Host", g_host[0], g_host),
                    },
                    CHAOS,
                    {SLAANESH}
            };
            s_registered = UnitFactory::Register("Seeker Chariots", factoryMethod);
        }
    }

    void SeekerChariots::onCharged() {
        SlaaneshBase::onCharged();

        // Multilating Blades
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0);
        for (auto ip : units) {
            if (Dice::RollD6() >= 2) {
                ip->applyDamage({0, Dice::RollD3()}, this);
            }
        }
    }

    int SeekerChariots::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // Slannesh
