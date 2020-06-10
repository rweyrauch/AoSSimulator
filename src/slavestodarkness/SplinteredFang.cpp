/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/SplinteredFang.h"

namespace SlavesToDarkness {
    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 40;
    static const int POINTS_PER_BLOCK = 70;
    static const int POINTS_MAX_UNIT_SIZE = 280;

    bool SplinteredFang::s_registered = false;

    Unit *SplinteredFang::Create(const ParameterList &parameters) {
        auto unit = new SplinteredFang();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, Ravagers);
        unit->setDamnedLegion(legion);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void SplinteredFang::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    SplinteredFang::Create,
                    SlavesToDarknessBase::ValueToString,
                    SlavesToDarknessBase::EnumStringToInt,
                    SplinteredFang::ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS}
            };
            s_registered = UnitFactory::Register("Splintered Fang", factoryMethod);
        }
    }

    SplinteredFang::SplinteredFang() :
            SlavesToDarknessBase("Splintered Fang", 6, WOUNDS, 5, 5, false),
            m_poisonedWeapons(Weapon::Type::Melee, "Poisoned Weapons", 1, 1, 4, 4, 0, 1),
            m_poisonedWeaponsLeader(Weapon::Type::Melee, "Poisoned Weapons (Trueblood)", 1, 2, 4, 4, 0, 1) {
        m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, CULTISTS, SPLINTERED_FANG};
        m_weapons = {&m_poisonedWeapons, &m_poisonedWeaponsLeader};
    }

    bool SplinteredFang::configure(int numModels) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        auto trueblood = new Model(BASESIZE, wounds());
        trueblood->addMeleeWeapon(&m_poisonedWeaponsLeader);
        trueblood->setName("Trueblood");
        addModel(trueblood);

        auto serpentCaller = new Model(BASESIZE, wounds());
        serpentCaller->addMeleeWeapon(&m_poisonedWeapons);
        serpentCaller->setName("Serpent Caller");
        addModel(serpentCaller);

        auto serpent = new Model(BASESIZE, WOUNDS + 1);
        serpent->addMeleeWeapon(&m_poisonedWeapons);
        serpent->setName("Serpent");
        addModel(serpent);

        for (auto i = 3; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_poisonedWeapons);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Wounds SplinteredFang::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // One Cut, One Kill
        if (hitRoll == 6) {
            return {0, 1};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    void SplinteredFang::onWounded() {
        Unit::onWounded();

        // Check for Serpent Caller
        for (const auto &ip : m_models) {
            if (ip->slain() && (ip->getName() == "Serpent Caller")) {
                m_hasSerpentCaller = false;
                break;
            }
        }
    }

    int SplinteredFang::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} //SlavesToDarkness