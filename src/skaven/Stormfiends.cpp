/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/Stormfiends.h>
#include <UnitFactory.h>
#include <array>

namespace Skaven {
    static const int g_basesize = 40;
    static const int g_wounds = 6;
    static const int g_minUnitSize = 3;
    static const int g_maxUnitSize = 6;
    static const int g_pointsPerBlock = 260;
    static const int g_pointsMaxUnitSize = 260 * 2;

    bool Stormfiends::s_registered = false;

    Stormfiends::Stormfiends(int numModels, WeaponOption1 weapon1, WeaponOption2 weapon2, WeaponOption3 weapon3, int points) :
            Skaventide("Stormfiends", 6, g_wounds, 6, 4, false, points),
            m_ratlingCannons(Weapon::Type::Missile, "Ratling Cannons", 12, RAND_3D6, 4, 3, -1, 1),
            m_windlaunchers(Weapon::Type::Missile, "Windlaunchers", 24, 3, 4, 4, -3, RAND_D3),
            m_warpfireProjectors(Weapon::Type::Missile, "Warpfire Projectors", 8, 0, 0, 0, 0, 0),
            m_doomfireGauntlets(Weapon::Type::Melee, "Doomflayer Gauntlets", 1, RAND_2D3, 3, 3, -2, RAND_D3),
            m_grinderfists(Weapon::Type::Melee, "Grinderfists", 1, 4, 4, 3, -2, 2),
            m_shockGauntlets(Weapon::Type::Melee, "Shock Gauntlets", 1, 4, 4, 3, -1, 2),
            m_clubbingBlows(Weapon::Type::Melee, "Clubbing Blows", 1, 4, 4, 3, 0, 2) {
        m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, CLANS_MOULDER, CLANS_SKRYRE, STORMFIENDS};
        m_weapons = {&m_ratlingCannons, &m_windlaunchers, &m_warpfireProjectors, &m_doomfireGauntlets, &m_grinderfists,
                     &m_shockGauntlets, &m_clubbingBlows};

        m_weapons1 = weapon1;
        m_weapons2 = weapon2;
        m_weapons3 = weapon3;

        int numWeapon1 = numModels / 3;

        for (auto i = 0; i < numWeapon1; i++) {
            auto model = new Model(g_basesize, wounds());
            if (weapon1 == Warpfire_Projectors) {
                model->addMissileWeapon(&m_warpfireProjectors);
            } else if (weapon1 == Windlaunchers) {
                model->addMissileWeapon(&m_windlaunchers);
            }
            model->addMeleeWeapon(&m_clubbingBlows);
            addModel(model);
        }

        int numWeapon2 = numModels / 3;
        for (auto i = 0; i < numWeapon2; i++) {
            auto model = new Model(g_basesize, wounds());
            if (weapon2 == Grinderfists) {
                model->addMeleeWeapon(&m_grinderfists);
            } else if (weapon2 == Ratling_Cannons) {
                model->addMissileWeapon(&m_ratlingCannons);
                model->addMeleeWeapon(&m_clubbingBlows);
            }
            addModel(model);
        }

        int numWeapon3 = numModels - (numWeapon1 + numWeapon2);
        for (auto i = 0; i < numWeapon3; i++) {
            // Warpstone-laced Armour => +1 wounds
            auto model = new Model(g_basesize, g_wounds + 1);
            if (weapon3 == Doomflayer_Gauntlets) {
                model->addMeleeWeapon(&m_doomfireGauntlets);
            } else if (weapon3 == Shock_Gauntlets) {
                model->addMeleeWeapon(&m_shockGauntlets);
            }
            addModel(model);
        }
    }

    Unit *Stormfiends::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto weapon1 = (WeaponOption1) GetEnumParam("Weapon A", parameters, Warpfire_Projectors);
        auto weapon2 = (WeaponOption2) GetEnumParam("Weapon B", parameters, Grinderfists);
        auto weapon3 = (WeaponOption3) GetEnumParam("Weapon C", parameters, Doomflayer_Gauntlets);
        return new Stormfiends(numModels, weapon1, weapon2, weapon3, ComputePoints(parameters));
    }

    void Stormfiends::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weaponA = {Warpfire_Projectors, Windlaunchers};
            static const std::array<int, 2> weaponB = {Grinderfists, Ratling_Cannons};
            static const std::array<int, 2> weaponC = {Doomflayer_Gauntlets, Shock_Gauntlets};
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Weapon A", Warpfire_Projectors, weaponA),
                            EnumParameter("Weapon B", Grinderfists, weaponB),
                            EnumParameter("Weapon C", Doomflayer_Gauntlets, weaponC),
                    },
                    CHAOS,
                    {SKAVEN}
            };
            s_registered = UnitFactory::Register("Stormfiends", factoryMethod);
        }
    }

    std::string Stormfiends::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapon A") {
            if (parameter.intValue == Warpfire_Projectors) {
                return "Warpfire Projectors";
            } else if (parameter.intValue == Windlaunchers) {
                return "Windlaunchers";
            }
        } else if (std::string(parameter.name) == "Weapon B") {
            if (parameter.intValue == Grinderfists) {
                return "Grinderfists";
            } else if (parameter.intValue == Ratling_Cannons) {
                return "Ratling Cannons";
            }
        } else if (std::string(parameter.name) == "Weapon C") {
            if (parameter.intValue == Doomflayer_Gauntlets) {
                return "Doomflayer Gauntlets";
            } else if (parameter.intValue == Shock_Gauntlets) {
                return "Shock Gauntlets";
            }
        }

        return ParameterValueToString(parameter);
    }

    int Stormfiends::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Warpfire Projectors") {
            return Warpfire_Projectors;
        } else if (enumString == "Windlaunchers") {
            return Windlaunchers;
        } else if (enumString == "Grinderfists") {
            return Grinderfists;
        } else if (enumString == "Ratling Cannons") {
            return Ratling_Cannons;
        } else if (enumString == "Doomflayer Gauntlets") {
            return Doomflayer_Gauntlets;
        } else if (enumString == "Shock Gauntlets") {
            return Shock_Gauntlets;
        }
        return 0;
    }

    int Stormfiends::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    int Stormfiends::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Skaventide::toHitModifier(weapon, target);

        // Doomflayer Gauntlets
        if (m_charged && (weapon->name() == m_doomfireGauntlets.name())) mod++;

        // Windlaunchers
        if ((weapon->name() == m_windlaunchers.name()) && (target->remainingModels() >= 10)) mod++;

        return mod;
    }

    int Stormfiends::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        // Shock Gauntlets
        if ((unmodifiedHitRoll == 6) && (weapon->name() == m_shockGauntlets.name())) {
            return Dice::RollD6();
        }
        return Unit::generateHits(unmodifiedHitRoll, weapon, unit);
    }

} // namespace Skaven
