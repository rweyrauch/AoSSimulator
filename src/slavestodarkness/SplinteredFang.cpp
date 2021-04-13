/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/SplinteredFang.h"
#include "SlavesToDarknessPrivate.h"

namespace SlavesToDarkness {
    static const int g_basesize = 32;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 40;
    static const int g_pointsPerBlock = 70;
    static const int g_pointsMaxUnitSize = 280;

    bool SplinteredFang::s_registered = false;

    Unit *SplinteredFang::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        return new SplinteredFang(legion, numModels);
    }

    void SplinteredFang::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    SplinteredFang::Create,
                    SlavesToDarknessBase::ValueToString,
                    SlavesToDarknessBase::EnumStringToInt,
                    SplinteredFang::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS}
            };
            s_registered = UnitFactory::Register("Splintered Fang", factoryMethod);
        }
    }

    SplinteredFang::SplinteredFang(DamnedLegion legion, int numModels) :
            SlavesToDarknessBase("Splintered Fang", 6, g_wounds, 5, 5, false),
            m_poisonedWeapons(Weapon::Type::Melee, "Poisoned Weapons", 1, 1, 4, 4, 0, 1),
            m_poisonedWeaponsLeader(Weapon::Type::Melee, "Poisoned Weapons (Trueblood)", 1, 2, 4, 4, 0, 1) {
        m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, CULTISTS, SPLINTERED_FANG};
        m_weapons = {&m_poisonedWeapons, &m_poisonedWeaponsLeader};

        setDamnedLegion(legion);

        auto trueblood = new Model(g_basesize, wounds());
        trueblood->addMeleeWeapon(&m_poisonedWeaponsLeader);
        trueblood->setName("Trueblood");
        addModel(trueblood);

        auto serpentCaller = new Model(g_basesize, wounds());
        serpentCaller->addMeleeWeapon(&m_poisonedWeapons);
        serpentCaller->setName("Serpent Caller");
        addModel(serpentCaller);

        auto serpent = new Model(g_basesize, g_wounds + 1);
        serpent->addMeleeWeapon(&m_poisonedWeapons);
        serpent->setName("Serpent");
        addModel(serpent);

        for (auto i = 3; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_poisonedWeapons);
            addModel(model);
        }

        m_points = ComputePoints(numModels);
    }

    Wounds SplinteredFang::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // One Cut, One Kill
        if (hitRoll == 6) {
            return {0, 1};
        }
        return SlavesToDarknessBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int SplinteredFang::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    void SplinteredFang::onStartHero(PlayerId player) {
        SlavesToDarknessBase::onStartHero(player);

        // Snake Charmer
        if (owningPlayer() == player) {
            if (isNamedModelAlive("Snake Charmer") && !isNamedModelAlive("Serpent")) {
                for (const auto &m : m_models) {
                    if ((m->getName() == "Serpent") && (m->slain())) {
                        m->restore();
                        break;
                    }
                }
            }
        }
    }

} //SlavesToDarkness