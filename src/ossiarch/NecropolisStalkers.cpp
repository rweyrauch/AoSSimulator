/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "ossiarch/NecropolisStalkers.h"
#include "OssiarchBonereaperPrivate.h"

namespace OssiarchBonereapers {
    static const int g_basesize = 50;
    static const int g_wounds = 4;
    static const int g_minUnitSize = 3;
    static const int g_maxUnitSize = 6;
    static const int g_pointsPerBlock = 180;
    static const int g_pointsMaxUnitSize = g_pointsPerBlock * 3;

    bool NecropolisStalkers::s_registered = false;

    Unit *NecropolisStalkers::Create(const ParameterList &parameters) {
        auto unit = new NecropolisStalkers();

        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        int numFalchions = GetIntParam("Dread Falchions", parameters, 1);

        auto legion = (Legion) GetEnumParam("Legion", parameters, g_legion[0]);
        unit->setLegion(legion);

        bool ok = unit->configure(numModels, numFalchions);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string NecropolisStalkers::ValueToString(const Parameter &parameter) {
        return OssiarchBonereaperBase::ValueToString(parameter);
    }

    int NecropolisStalkers::EnumStringToInt(const std::string &enumString) {
        return OssiarchBonereaperBase::EnumStringToInt(enumString);
    }

    void NecropolisStalkers::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    NecropolisStalkers::Create,
                    NecropolisStalkers::ValueToString,
                    NecropolisStalkers::EnumStringToInt,
                    NecropolisStalkers::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            IntegerParameter("Dread Falchions", 1, 0, g_maxUnitSize / 3, 1),
                            EnumParameter("Legion", g_legion[0], g_legion),
                    },
                    DEATH,
                    {OSSIARCH_BONEREAPERS}
            };
            s_registered = UnitFactory::Register("Necropolis Stalkers", factoryMethod);
        }
    }

    NecropolisStalkers::NecropolisStalkers() :
            OssiarchBonereaperBase("Necropolis Stalkers", 6, g_wounds, 10, 4, false),
            m_falchions(Weapon::Type::Melee, "Dread Falchions", 1, 3, 4, 3, -2, 2),
            m_blades(Weapon::Type::Melee, "Spirit Blades", 1, 5, 3, 3, -1, 1) {
        m_keywords = {DEATH, OSSIARCH_BONEREAPERS, HEKATOS, NECROPOLIS_STALKERS};
        m_weapons = {&m_falchions, &m_blades};
    }

    bool NecropolisStalkers::configure(int numModels, int numFalchions) {
        // validate inputs
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            // Invalid model count.
            return false;
        }
        const int maxFalchions = numModels / 3;
        if (numFalchions > maxFalchions) {
            // Invalid weapon configuration.
            return false;
        }

        for (auto i = 0; i < numFalchions; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_falchions);
            addModel(model);
        }
        for (auto i = numFalchions; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_blades);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    void NecropolisStalkers::onStartCombat(PlayerId player) {
        Unit::onStartCombat(player);

        // Select active aspect (randomly for now)
        m_activeAspect = (Aspect) Dice::RollD4();
    }

    Rerolls NecropolisStalkers::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        if (m_activeAspect == Blade_Strike) return Reroll_Failed;
        return Unit::toHitRerolls(weapon, target);
    }

    Rerolls NecropolisStalkers::toWoundRerolls(const Weapon *weapon, const Unit *target) const {
        if (m_activeAspect == Destroyer) return Reroll_Failed;
        return Unit::toWoundRerolls(weapon, target);
    }

    Rerolls NecropolisStalkers::toSaveRerolls(const Weapon *weapon) const {
        if (m_activeAspect == Blade_Parry) return Reroll_Failed;
        return Unit::toSaveRerolls(weapon);
    }

    Wounds
    NecropolisStalkers::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        auto damage = Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
        if (m_activeAspect == Precision) damage.normal++;
        return damage;
    }

    int NecropolisStalkers::weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        auto rend = Unit::weaponRend(weapon, target, hitRoll, woundRoll);
        if (m_activeAspect == Precision) rend--;
        return rend;
    }

    int NecropolisStalkers::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace OssiarchBonereapers
