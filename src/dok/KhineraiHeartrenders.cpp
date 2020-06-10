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

namespace DaughtersOfKhaine {
    static const int BASESIZE = 40;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 90;
    static const int POINTS_MAX_UNIT_SIZE = 90 * 4;

    bool KhineraiHeartrenders::s_registered = false;

    KhineraiHeartrenders::KhineraiHeartrenders() :
            DaughterOfKhaine("Khinerai Heartrenders", 14, WOUNDS, 7, 6, true),
            m_barbedJavelinMissile(Weapon::Type::Missile, "Barbed Javelin", 12, 1, 3, 3, -1, 1),
            m_barbedJavelin(Weapon::Type::Melee, "Barbed Javelin", 2, 1, 4, 4, -1, 1),
            m_barbedJavelinShrykeMissile(Weapon::Type::Missile, "Barbed Javelin", 12, 1, 2, 3, -1, 1),
            m_barbedJavelinShryke(Weapon::Type::Melee, "Barbed Javelin", 2, 1, 3, 4, -1, 1) {
        m_keywords = {ORDER, DAUGHTERS_OF_KHAINE, KHINERAI_HARPIES, KHINERAI_HEARTRENDERS};
        m_weapons = {&m_barbedJavelinMissile, &m_barbedJavelin, &m_barbedJavelinShrykeMissile, &m_barbedJavelinShryke};

        // Death From Above
        m_runAndShoot = true;
    }

    bool KhineraiHeartrenders::configure(int numModels) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        auto shryke = new Model(BASESIZE, wounds());
        shryke->addMissileWeapon(&m_barbedJavelinShrykeMissile);
        shryke->addMeleeWeapon(&m_barbedJavelinShryke);
        addModel(shryke);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMissileWeapon(&m_barbedJavelinMissile);
            model->addMeleeWeapon(&m_barbedJavelin);
            addModel(model);
        }

        m_setupInRound = 0;

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *KhineraiHeartrenders::Create(const ParameterList &parameters) {
        auto unit = new KhineraiHeartrenders();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

        auto temple = (Temple)GetEnumParam("Temple", parameters, g_temple[0]);
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
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
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

    int KhineraiHeartrenders::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

    int KhineraiHeartrenders::toSaveModifier(const Weapon *weapon) const {
        auto mod = Unit::toSaveModifier(weapon);

        // Heartpiercer Shield
        if (!weapon->isMissile()) mod++;

        return mod;
    }

    int KhineraiHeartrenders::weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Death From Above
        if ((m_setupInRound == m_battleRound) && weapon->isMissile() &&
            (weapon->name() == m_barbedJavelinMissile.name()))
            return -2;
        return Unit::weaponRend(weapon, target, hitRoll, woundRoll);
    }

    void KhineraiHeartrenders::onRestore() {
        Unit::onRestore();

        m_setupInRound = 0;
    }

} // namespace DaughtersOfKhaine