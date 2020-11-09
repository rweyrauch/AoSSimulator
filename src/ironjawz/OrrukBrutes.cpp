/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <ironjawz/OrrukBrutes.h>
#include <UnitFactory.h>
#include "IronjawzPrivate.h"

namespace Ironjawz {
    static const int g_basesize = 40;
    static const int g_wounds = 3;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 20;
    static const int g_pointsPerBlock = 130;
    static const int g_pointsMaxUnitSize = 520;

    bool OrrukBrutes::s_registered = false;

    OrrukBrutes::OrrukBrutes() :
            Ironjawz("Orruk Brutes", 4, g_wounds, 6, 4, false),
            m_twoBruteChoppas(Weapon::Type::Melee, "Pair of Brute Choppas", 1, 4, 3, 3, -1, 1),
            m_gorehacka(Weapon::Type::Melee, "Jagged Gore-hacka", 2, 3, 3, 3, -1, 1),
            m_gorechoppa(Weapon::Type::Melee, "Gore-choppa", 2, 3, 4, 3, -1, 2),
            m_bossChoppa(Weapon::Type::Melee, "Boss Choppa", 1, 3, 3, 3, -1, 2),
            m_bossKlawAndBruteSmasha(Weapon::Type::Melee, "Boss Klaw and Brute Smasha", 1, 4, 4, 3, -1, 2) {
        m_keywords = {DESTRUCTION, ORRUK, IRONJAWZ, BRUTES};
        m_weapons = {&m_twoBruteChoppas, &m_gorehacka, &m_gorechoppa, &m_bossChoppa, &m_bossKlawAndBruteSmasha};
    }

    bool OrrukBrutes::configure(int numModels, WeaponOption weapons, int numGoreChoppas, BossWeaponOption bossWeapon) {
        // validate inputs
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            // Invalid model count.
            return false;
        }
        const int maxGoreChoppas = numModels / 5;
        if (numGoreChoppas > maxGoreChoppas) {
            // Invalid weapon configuration.
            return false;
        }

        auto bossModel = new Model(g_basesize, wounds());
        if (bossWeapon == Boss_Choppa) {
            bossModel->addMeleeWeapon(&m_bossChoppa);
        } else if (bossWeapon == Boss_Klaw) {
            bossModel->addMeleeWeapon(&m_bossKlawAndBruteSmasha);
        }
        addModel(bossModel);

        for (auto i = 0; i < numGoreChoppas; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_gorechoppa);
            addModel(model);
        }

        int currentModelCount = (int) m_models.size();
        for (auto i = currentModelCount; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            if (weapons == Two_Brute_Choppas) {
                model->addMeleeWeapon(&m_twoBruteChoppas);
            } else if (weapons == Jagged_Gorehacka) {
                model->addMeleeWeapon(&m_gorehacka);
            }
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *OrrukBrutes::Create(const ParameterList &parameters) {
        auto unit = new OrrukBrutes();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        WeaponOption weapons = (WeaponOption) GetEnumParam("Weapons", parameters, Two_Brute_Choppas);
        int numGoreChoppas = GetIntParam("Gore Choppas", parameters, 0);
        BossWeaponOption bossWeapon = (BossWeaponOption) GetEnumParam("Boss Weapon", parameters, Boss_Choppa);

        auto warclan = (Warclan) GetEnumParam("Warclan", parameters, g_warclan[0]);
        unit->setWarclan(warclan);

        bool ok = unit->configure(numModels, weapons, numGoreChoppas, bossWeapon);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void OrrukBrutes::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Two_Brute_Choppas, Jagged_Gorehacka};
            static const std::array<int, 2> bossWeapons = {Boss_Choppa, Boss_Klaw};
            static FactoryMethod factoryMethod = {
                    OrrukBrutes::Create,
                    OrrukBrutes::ValueToString,
                    OrrukBrutes::EnumStringToInt,
                    OrrukBrutes::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Weapons", Two_Brute_Choppas, weapons),
                            IntegerParameter("Gore Choppas", 0, 0, g_maxUnitSize / 5, 1),
                            EnumParameter("Boss Weapon", Boss_Choppa, bossWeapons),
                            EnumParameter("Warclan", g_warclan[0], g_warclan),
                    },
                    DESTRUCTION,
                    {IRONJAWZ}
            };
            s_registered = UnitFactory::Register("Orruk Brutes", factoryMethod);
        }
    }

    std::string OrrukBrutes::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == Two_Brute_Choppas) {
                return "Two Brute Choppas";
            } else if (parameter.intValue == Jagged_Gorehacka) {
                return "Jagged Gore-hacka";
            }
        }

        if (std::string(parameter.name) == "Boss Weapon") {
            if (parameter.intValue == Boss_Choppa) {
                return "Boss Choppa";
            } else if (parameter.intValue == Boss_Klaw) {
                return "Boss Klaw";
            }
        }

        return Ironjawz::ValueToString(parameter);
    }

    int OrrukBrutes::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Two Brute Choppas") {
            return Two_Brute_Choppas;
        } else if (enumString == "Jagged Gore-hacka") {
            return Jagged_Gorehacka;
        } else if (enumString == "Boss Choppa") {
            return Boss_Choppa;
        } else if (enumString == "Boss Klaw") {
            return Boss_Klaw;
        }

        return Ironjawz::EnumStringToInt(enumString);
    }

    Rerolls OrrukBrutes::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Duff Up da Big Thing
        if (target->wounds() >= 4) {
            return Reroll_Failed;
        }
        return Ironjawz::toHitRerolls(weapon, target);
    }

    int OrrukBrutes::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace Ironjawz
