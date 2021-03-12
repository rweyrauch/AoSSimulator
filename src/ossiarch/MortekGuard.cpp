/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "ossiarch/MortekGuard.h"
#include "OssiarchBonereaperPrivate.h"

namespace OssiarchBonereapers {
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 40;
    static const int g_pointsPerBlock = 130;
    static const int g_pointsMaxUnitSize = 440;

    bool MortekGuard::s_registered = false;

    Unit *MortekGuard::Create(const ParameterList &parameters) {
        auto unit = new MortekGuard();

        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto weapons = (WeaponOption) GetEnumParam("Weapons", parameters, Nadirite_Blade_And_Shield);
        bool necrophoros = GetBoolParam("Necrophoros", parameters, true);
        int numGreatblades = GetIntParam("Soulcleaver Greatblade", parameters, 1);

        auto legion = (Legion) GetEnumParam("Legion", parameters, g_legion[0]);
        unit->setLegion(legion);

        bool ok = unit->configure(numModels, weapons, numGreatblades, necrophoros);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string MortekGuard::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == Nadirate_Spear_And_Shield) {
                return "Nadirate Spear and Shield";
            } else if (parameter.intValue == Nadirite_Blade_And_Shield) {
                return "Nadirite Blade and Shield";
            }
        }
        return OssiarchBonereaperBase::ValueToString(parameter);
    }

    int MortekGuard::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Nadirite Blade and Shield") return Nadirite_Blade_And_Shield;
        else if (enumString == "Nadirate Spear and Shield") return Nadirate_Spear_And_Shield;

        return OssiarchBonereaperBase::EnumStringToInt(enumString);
    }

    void MortekGuard::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Nadirite_Blade_And_Shield, Nadirate_Spear_And_Shield};
            static FactoryMethod factoryMethod = {
                    MortekGuard::Create,
                    MortekGuard::ValueToString,
                    MortekGuard::EnumStringToInt,
                    MortekGuard::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            BoolParameter("Necrophoros"),
                            EnumParameter("Weapons", Nadirite_Blade_And_Shield, weapons),
                            IntegerParameter("Soulcleaver Greatblade", 1, 0, g_maxUnitSize / 3, 1),
                            EnumParameter("Legion", g_legion[0], g_legion),
                    },
                    DEATH,
                    {OSSIARCH_BONEREAPERS}
            };
            s_registered = UnitFactory::Register("Mortek Guard", factoryMethod);
        }
    }

    MortekGuard::MortekGuard() :
            OssiarchBonereaperBase("Mortek Guard", 4, g_wounds, 10, 4, false),
            m_blade(Weapon::Type::Melee, "Nadirite Blade", 1, 2, 3, 4, -1, 1),
            m_spear(Weapon::Type::Melee, "Nadirite Spear", 2, 2, 3, 4, 0, 1),
            m_greatblade(Weapon::Type::Melee, "Soulcleaver Greatblade", 1, 2, 3, 3, -1, 1),
            m_bladeHekatos(Weapon::Type::Melee, "Nadirite Blade", 1, 3, 3, 4, -1, 1),
            m_spearHekatos(Weapon::Type::Melee, "Nadirite Spear", 2, 3, 3, 4, 0, 1),
            m_greatbladeHekatos(Weapon::Type::Melee, "Soulcleaver Greatblade", 1, 3, 3, 3, -1, 1) {
        m_keywords = {DEATH, OSSIARCH_BONEREAPERS, MORTEK_GUARD};
        m_weapons = {&m_blade, &m_spear, &m_greatblade, &m_bladeHekatos, &m_spearHekatos, &m_greatbladeHekatos};
        m_battleFieldRole = Role::Battleline;
    }

    bool MortekGuard::configure(int numModels, WeaponOption option, int numGreatblades, bool necrophoros) {
        // validate inputs
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            // Invalid model count.
            return false;
        }
        const int maxGreatblades = numModels / 10;
        if (numGreatblades > maxGreatblades) {
            // Invalid weapon configuration.
            return false;
        }

        auto hekatos = new Model(g_basesize, wounds());
        if (numGreatblades) {
            hekatos->addMeleeWeapon(&m_greatbladeHekatos);
            numGreatblades--;
        } else if (option == Nadirite_Blade_And_Shield) {
            hekatos->addMeleeWeapon(&m_bladeHekatos);
        } else if (option == Nadirate_Spear_And_Shield) {
            hekatos->addMeleeWeapon(&m_spearHekatos);
        }
        addModel(hekatos);
        for (auto i = 0; i < numGreatblades; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_greatblade);
            addModel(model);
        }

        int currentModelCount = (int) m_models.size();
        for (auto i = currentModelCount; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            if (option == Nadirite_Blade_And_Shield) {
                model->addMeleeWeapon(&m_bladeHekatos);
            } else if (option == Nadirate_Spear_And_Shield) {
                model->addMeleeWeapon(&m_spearHekatos);
            }
            addModel(model);
        }

        m_necrophoros = necrophoros;

        m_points = ComputePoints(numModels);

        return true;
    }

    int MortekGuard::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        if ((unmodifiedHitRoll == 6) &&
            ((weapon->name() == m_blade.name()) || (weapon->name() == m_spear.name())))
            return 2;
        return Unit::generateHits(unmodifiedHitRoll, weapon, unit);
    }

    int MortekGuard::runModifier() const {
        auto mod = Unit::runModifier();
        if (m_necrophoros) mod++;
        return mod;
    }

    int MortekGuard::chargeModifier() const {
        auto mod = Unit::chargeModifier();
        if (m_necrophoros) mod++;
        return mod;
    }

    int MortekGuard::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace OssiarchBonereapers
