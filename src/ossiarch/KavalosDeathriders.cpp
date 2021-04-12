/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "ossiarch/KavalosDeathriders.h"
#include "OssiarchBonereaperPrivate.h"

namespace OssiarchBonereapers {
    static const int g_basesize = 60; // x35
    static const int g_wounds = 3;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 15;
    static const int g_pointsPerBlock = 180;
    static const int g_pointsMaxUnitSize = 480;

    bool KavalosDeathriders::s_registered = false;

    Unit *KavalosDeathriders::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto weapons = (WeaponOption) GetEnumParam("Weapons", parameters, Nadirite_Blade_And_Shield);
        bool necrophoros = GetBoolParam("Necrophoros", parameters, true);
        auto legion = (Legion) GetEnumParam("Legion", parameters, g_legion[0]);

        return new KavalosDeathriders(legion, numModels, weapons, necrophoros);
    }

    std::string KavalosDeathriders::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == Nadirate_Spear_And_Shield) {
                return "Nadirate Spear and Shield";
            } else if (parameter.intValue == Nadirite_Blade_And_Shield) {
                return "Nadirite Blade and Shield";
            }
        }
        return OssiarchBonereaperBase::ValueToString(parameter);
    }

    int KavalosDeathriders::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Nadirite Blade and Shield") return Nadirite_Blade_And_Shield;
        else if (enumString == "Nadirate Spear and Shield") return Nadirate_Spear_And_Shield;

        return OssiarchBonereaperBase::EnumStringToInt(enumString);
    }

    void KavalosDeathriders::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Nadirite_Blade_And_Shield, Nadirate_Spear_And_Shield};
            static FactoryMethod factoryMethod = {
                    KavalosDeathriders::Create,
                    KavalosDeathriders::ValueToString,
                    KavalosDeathriders::EnumStringToInt,
                    KavalosDeathriders::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            BoolParameter("Necrophoros"),
                            EnumParameter("Weapons", Nadirite_Blade_And_Shield, weapons),
                            EnumParameter("Legion", g_legion[0], g_legion),
                    },
                    DEATH,
                    {OSSIARCH_BONEREAPERS}
            };
            s_registered = UnitFactory::Register("Kavalos Deathriders", factoryMethod);
        }
    }

    KavalosDeathriders::KavalosDeathriders(Legion legion, int numModels, WeaponOption option, bool necrophoros) :
            OssiarchBonereaperBase("Kavalos Deathriders", 12, g_wounds, 10, 4, false) {
        m_keywords = {DEATH, OSSIARCH_BONEREAPERS, KAVALOS_DEATHRIDERS};
        m_weapons = {&m_blade, &m_spear, &m_bladeHekatos, &m_spearHekatos, &m_hoovesAndTeeth};
        m_battleFieldRole = Role::Battleline;
        m_hasMount = true;
        m_hoovesAndTeeth.setMount(true);

        setLegion(legion);

        auto hekatos = new Model(g_basesize, wounds());
        if (option == Nadirite_Blade_And_Shield) {
            hekatos->addMeleeWeapon(&m_bladeHekatos);
        } else if (option == Nadirate_Spear_And_Shield) {
            hekatos->addMeleeWeapon(&m_spearHekatos);
        }
        hekatos->addMeleeWeapon(&m_hoovesAndTeeth);
        addModel(hekatos);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            if (option == Nadirite_Blade_And_Shield) {
                model->addMeleeWeapon(&m_bladeHekatos);
            } else if (option == Nadirate_Spear_And_Shield) {
                model->addMeleeWeapon(&m_spearHekatos);
            }
            model->addMeleeWeapon(&m_hoovesAndTeeth);
            addModel(model);
        }

        m_necrophoros = necrophoros;

        m_points = ComputePoints(numModels);
    }

    int KavalosDeathriders::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        if (m_charged && (weapon->name() == m_spear.name()) && (unmodifiedHitRoll >= 5)) return 2;
        if ((unmodifiedHitRoll == 6) &&
            ((weapon->name() == m_blade.name()) || (weapon->name() == m_spear.name())))
            return 2;
        return OssiarchBonereaperBase::generateHits(unmodifiedHitRoll, weapon, unit);
    }

    int KavalosDeathriders::runModifier() const {
        auto mod = OssiarchBonereaperBase::runModifier();
        if (m_necrophoros) mod++;
        return mod;
    }

    int KavalosDeathriders::chargeModifier() const {
        auto mod = OssiarchBonereaperBase::chargeModifier();
        if (m_necrophoros) mod++;
        return mod;
    }

    int KavalosDeathriders::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace OssiarchBonereapers
