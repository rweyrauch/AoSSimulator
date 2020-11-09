/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <dok/KhineraiLifetakers.h>
#include <UnitFactory.h>
#include <iostream>
#include "DaughterOfKhainePrivate.h"

namespace DaughtersOfKhaine {
    static const int g_basesize = 40;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 20;
    static const int g_pointsPerBlock = 80;
    static const int g_pointsMaxUnitSize = 280;

    bool KhineraiLifetakers::s_registered = false;

    KhineraiLifetakers::KhineraiLifetakers() :
            DaughterOfKhaine("Khinerai Lifetakers", 14, g_wounds, 7, 6, true),
            m_barbedSickle(Weapon::Type::Melee, "Barbed Sickle", 1, 2, 3, 4, 0, 1),
            m_barbedSickleHarridynn(Weapon::Type::Melee, "Barbed Sickle", 1, 2, 2, 4, 0, 1) {
        m_keywords = {ORDER, DAUGHTERS_OF_KHAINE, KHINERAI_HARPIES, KHINERAI_LIFETAKERS};
        m_weapons = {&m_barbedSickle, &m_barbedSickleHarridynn};
    }

    bool KhineraiLifetakers::configure(int numModels) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        auto harridynn = new Model(g_basesize, wounds());
        harridynn->addMeleeWeapon(&m_barbedSickleHarridynn);
        addModel(harridynn);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_barbedSickle);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *KhineraiLifetakers::Create(const ParameterList &parameters) {
        auto unit = new KhineraiLifetakers();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);

        auto temple = (Temple)GetEnumParam("Temple", parameters, g_temple[0]);
        unit->setTemple(temple);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void KhineraiLifetakers::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    DaughterOfKhaine::ValueToString,
                    DaughterOfKhaine::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Temple", g_temple[0], g_temple)
                    },
                    ORDER,
                    {DAUGHTERS_OF_KHAINE}
            };
            s_registered = UnitFactory::Register("Khinerai Lifetakers", factoryMethod);
        }
    }

    void KhineraiLifetakers::onBeginTurn(int battleRound) {
        DaughterOfKhaine::onBeginTurn(battleRound);

        // reset weapon damage (if previously charged)
        m_barbedSickle.setDamage(m_barbedSickle.damage());
        m_barbedSickleHarridynn.setDamage(m_barbedSickleHarridynn.damage());
    }

    void KhineraiLifetakers::onCharged() {
        DaughterOfKhaine::onCharged();

        // Death on the Wind
        m_barbedSickle.setDamage(m_barbedSickle.damage() + 1);
        m_barbedSickleHarridynn.setDamage(m_barbedSickleHarridynn.damage() + 1);
    }

    Wounds KhineraiLifetakers::computeReturnedDamage(const Weapon *weapon, int saveRoll) const {
        auto wounds = DaughterOfKhaine::computeReturnedDamage(weapon, saveRoll);
        // Heartpiercer Shield
        if (!weapon->isMissile()) {
            // 1 mortal wound for each save of a 6
            wounds += {0, 1};
        }
        return wounds;
    }

    int KhineraiLifetakers::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    int KhineraiLifetakers::toSaveModifier(const Weapon *weapon) const {
        auto mod = Unit::toSaveModifier(weapon);

        // Heartpiercer Shield
        if (!weapon->isMissile()) mod++;

        return mod;
    }

} // namespace DaughtersOfKhaine