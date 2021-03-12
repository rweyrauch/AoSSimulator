/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/SymbareshTwinsouls.h>
#include <Weapon.h>
#include <UnitFactory.h>
#include "SlaaneshPrivate.h"

namespace Slaanesh {
    static const int g_basesize = 32;
    static const int g_wounds = 2;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 20;
    static const int g_pointsPerBlock = 170;
    static const int g_pointsMaxUnitSize = (g_maxUnitSize / g_minUnitSize) * g_pointsPerBlock;

    bool SymbareshTwinsouls::s_registered = false;

    SymbareshTwinsouls::SymbareshTwinsouls() :
            SlaaneshBase("Symbaresh Twinsouls", 8, g_wounds, 7, 4, false),
            m_blades(Weapon::Type::Missile, "Merciless Blades", 2, 3, 4, 3, 0, 2),
            m_bladesEgopomp(Weapon::Type::Melee, "Merciless Blades", 2, 4, 4, 3, 0, 2) {
        m_keywords = {CHAOS, MORTAL, SLAANESH, HEDONITE, SYMBARESH_TWINSOULS};
        m_weapons = {&m_blades, &m_bladesEgopomp};
    }

    bool SymbareshTwinsouls::configure(int numModels) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        auto egopomp = new Model(g_basesize, wounds());
        egopomp->addMeleeWeapon(&m_bladesEgopomp);
        egopomp->setName("Egopomp");
        addModel(egopomp);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_blades);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *SymbareshTwinsouls::Create(const ParameterList &parameters) {
        auto unit = new SymbareshTwinsouls();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);

        auto host = (Host) GetEnumParam("Host", parameters, g_host[0]);
        unit->setHost(host);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void SymbareshTwinsouls::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SlaaneshBase::ValueToString,
                    SlaaneshBase::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Host", g_host[0], g_host),
                    },
                    CHAOS,
                    {SLAANESH}
            };
            s_registered = UnitFactory::Register("Symbaresh Twinsouls", factoryMethod);
        }
    }

    int SymbareshTwinsouls::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    Rerolls SymbareshTwinsouls::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Ego-driven Excess
        if (!m_fiendishReflexesActive && !weapon->isMissile()) {
            return Rerolls::Failed;
        }
        return SlaaneshBase::toHitRerolls(weapon, target);
    }

    Wounds SymbareshTwinsouls::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        if (m_fiendishReflexesActive) {
            // Fiendish Reflexes
            return ignoreWounds(wounds, 5);
        }
        return Unit::applyWoundSave(wounds, attackingUnit);
    }

    void SymbareshTwinsouls::onStartHero(PlayerId player) {
        EventInterface::onStartHero(player);

        // Fractured Souls
        m_fiendishReflexesActive = !m_fiendishReflexesActive;
    }

} // namespace Slaanesh