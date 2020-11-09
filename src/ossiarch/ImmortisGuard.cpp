/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "ossiarch/ImmortisGuard.h"
#include "OssiarchBonereaperPrivate.h"

namespace OssiarchBonereapers {
    static const int g_basesize = 50;
    static const int g_wounds = 4;
    static const int g_minUnitSize = 3;
    static const int g_maxUnitSize = 12;
    static const int g_pointsPerBlock = 180;
    static const int g_pointsMaxUnitSize = 720;

    bool ImmortisGuard::s_registered = false;

    Unit *ImmortisGuard::Create(const ParameterList &parameters) {
        auto unit = new ImmortisGuard();

        int numModels = GetIntParam("Models", parameters, g_minUnitSize);

        auto legion = (Legion) GetEnumParam("Legion", parameters, g_legion[0]);
        unit->setLegion(legion);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string ImmortisGuard::ValueToString(const Parameter &parameter) {
        return OssiarchBonereaperBase::ValueToString(parameter);
    }

    int ImmortisGuard::EnumStringToInt(const std::string &enumString) {
        return OssiarchBonereaperBase::EnumStringToInt(enumString);
    }

    void ImmortisGuard::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    ImmortisGuard::Create,
                    ImmortisGuard::ValueToString,
                    ImmortisGuard::EnumStringToInt,
                    ImmortisGuard::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Legion", g_legion[0], g_legion),
                    },
                    DEATH,
                    {OSSIARCH_BONEREAPERS}
            };
            s_registered = UnitFactory::Register("Immortis Guard", factoryMethod);
        }
    }

    ImmortisGuard::ImmortisGuard() :
            OssiarchBonereaperBase("Immortis Guard", 5, g_wounds, 10, 3, false),
            m_halberd(Weapon::Type::Melee, "Dread Halberd", 2, 2, 3, 3, -2, 2),
            m_battleShield(Weapon::Type::Melee, "Battle Shield", 1, 2, 4, 3, 0, 1) {
        m_keywords = {DEATH, OSSIARCH_BONEREAPERS, HEKATOS, IMMORTIS_GUARD};
        m_weapons = {&m_halberd, &m_battleShield};
    }

    bool ImmortisGuard::configure(int numModels) {
        // validate inputs
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            // Invalid model count.
            return false;
        }

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_halberd);
            model->addMeleeWeapon(&m_battleShield);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Wounds ImmortisGuard::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Deadly Combination
        if ((hitRoll == 6) && (weapon->name() == m_battleShield.name())) {
            return {weapon->damage(), 1};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int ImmortisGuard::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace OssiarchBonereapers
