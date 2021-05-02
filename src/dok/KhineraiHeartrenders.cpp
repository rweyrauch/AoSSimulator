/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <dok/KhineraiHeartrenders.h>
#include <UnitFactory.h>
#include <iostream>
#include "DaughterOfKhainePrivate.h"

namespace DaughtersOfKhaine {
    static const int g_basesize = 40;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 20;
    static const int g_pointsPerBlock = 80;
    static const int g_pointsMaxUnitSize = (g_maxUnitSize / g_minUnitSize) * g_pointsPerBlock;

    bool KhineraiHeartrenders::s_registered = false;

    KhineraiHeartrenders::KhineraiHeartrenders(int points) :
            DaughterOfKhaine("Khinerai Heartrenders", 14, g_wounds, 7, 6, true, points),
            m_barbedJavelinMissile(Weapon::Type::Missile, "Barbed Javelin", 12, 1, 3, 3, -1, 1),
            m_barbedJavelin(Weapon::Type::Melee, "Barbed Javelin", 2, 1, 4, 4, -1, 1),
            m_barbedJavelinShrykeMissile(Weapon::Type::Missile, "Barbed Javelin", 12, 2, 3, 3, -1, 1),
            m_barbedJavelinShryke(Weapon::Type::Melee, "Barbed Javelin", 2, 2, 4, 4, -1, 1) {
        m_keywords = {ORDER, DAUGHTERS_OF_KHAINE, KHINERAI_HARPIES, KHINERAI_HEARTRENDERS};
        m_weapons = {&m_barbedJavelinMissile, &m_barbedJavelin, &m_barbedJavelinShrykeMissile, &m_barbedJavelinShryke};

        // Death From Above
        m_runAndShoot = true;
    }

    bool KhineraiHeartrenders::configure(int numModels) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        auto shryke = new Model(g_basesize, wounds());
        shryke->addMissileWeapon(&m_barbedJavelinShrykeMissile);
        shryke->addMeleeWeapon(&m_barbedJavelinShryke);
        addModel(shryke);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_barbedJavelinMissile);
            model->addMeleeWeapon(&m_barbedJavelin);
            addModel(model);
        }

        m_setupInRound = 0;

        return true;
    }

    Unit *KhineraiHeartrenders::Create(const ParameterList &parameters) {
        auto unit = new KhineraiHeartrenders(ComputePoints(parameters));
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);

        auto temple = (Temple) GetEnumParam("Temple", parameters, g_temple[0]);
        unit->setTemple(temple);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void KhineraiHeartrenders::Init() {
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

    Wounds KhineraiHeartrenders::computeReturnedDamage(const Weapon *weapon, int saveRoll) const {
        auto wounds = DaughterOfKhaine::computeReturnedDamage(weapon, saveRoll);
        // Heartpiercer Shield
        if (!weapon->isMissile()) {
            // 1 mortal wound for each save of a 6
            wounds += {0, 1};
        }
        return wounds;
    }

    int KhineraiHeartrenders::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    int KhineraiHeartrenders::toSaveModifier(const Weapon *weapon, const Unit *attacker) const {
        auto mod = DaughterOfKhaine::toSaveModifier(weapon, attacker);

        // Heartpiercer Shield
        if (!weapon->isMissile()) mod++;

        return mod;
    }

    int KhineraiHeartrenders::weaponRend(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Death From Above
        if ((m_setupInRound == m_battleRound) && weapon->isMissile() &&
            (weapon->name() == m_barbedJavelinMissile.name()))
            return weapon->rend() - 1;
        return DaughterOfKhaine::weaponRend(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    void KhineraiHeartrenders::onRestore() {
        DaughterOfKhaine::onRestore();

        m_setupInRound = 0;
    }

} // namespace DaughtersOfKhaine