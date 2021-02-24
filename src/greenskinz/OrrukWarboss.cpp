/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <greenskinz/OrrukWarboss.h>
#include <UnitFactory.h>
#include <array>

namespace Greenskinz {
    static const int g_basesize = 32;
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 140;

    bool OrrukWarboss::s_registered = false;

    OrrukWarboss::OrrukWarboss() :
            Unit("Orruk Warboss", 5, g_wounds, 7, 4, false),
            m_bossChoppa(Weapon::Type::Melee, "Boss Choppa", 1, 6, 3, 3, -1, 1),
            m_massiveChoppa(Weapon::Type::Melee, "Massive Choppa", 1, 3, 4, 3, -2, RAND_D3),
            m_greatWaaaghBanner(Weapon::Type::Melee, "Great Waaagh! Banner", 2, 4, 4, 4, 0, 1),
            m_boarTusks(Weapon::Type::Melee, "War Boar's Tusks", 1, 2, 4, 4, 0, 1) {
        m_keywords = {DESTRUCTION, ORRUK, GREENSKINZ, HERO, ORRUK_WARBOSS};
        m_weapons = {&m_bossChoppa, &m_massiveChoppa, &m_greatWaaaghBanner, &m_boarTusks};
        m_battleFieldRole = Leader;
    }

    bool OrrukWarboss::configure(WeaponOption weapon, bool warboar) {
        auto model = new Model(g_basesize, wounds());

        m_weapon = weapon;

        switch (weapon) {
            case Boss_Choppa_And_Shield:
            case Paired_Boss_Choppas:
                model->addMeleeWeapon(&m_bossChoppa);
                break;
            case Massive_Choppa:
                model->addMeleeWeapon(&m_massiveChoppa);
                break;
            case Great_Waaagh_Banner:
                model->addMeleeWeapon(&m_greatWaaaghBanner);
                addKeyword(TOTEM);
                break;
            default:
                PLOG_INFO.printf("Unknown Orruk Warboss weapon %d", weapon);
                break;
        }
        if (warboar) {
            model->addMeleeWeapon(&m_boarTusks);
            m_move = 9;
        }
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *OrrukWarboss::Create(const ParameterList &parameters) {
        auto unit = new OrrukWarboss();
        WeaponOption weapon = (WeaponOption) GetEnumParam("Weapon", parameters, Boss_Choppa_And_Shield);
        bool warboar = GetBoolParam("War Boar", parameters, false);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure(weapon, warboar);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void OrrukWarboss::Init() {
        if (!s_registered) {
            static const std::array<int, 4> weapons = {Boss_Choppa_And_Shield, Paired_Boss_Choppas,
                                                       Massive_Choppa, Great_Waaagh_Banner};
            static FactoryMethod factoryMethod = {
                    OrrukWarboss::Create,
                    OrrukWarboss::ValueToString,
                    OrrukWarboss::EnumStringToInt,
                    OrrukWarboss::ComputePoints,
                    {
                            EnumParameter("Weapon", Boss_Choppa_And_Shield, weapons),
                            BoolParameter("War Boar"),
                            BoolParameter("General")
                    },
                    DESTRUCTION,
                    {GREENSKINZ}
            };
            s_registered = UnitFactory::Register("Orruk Warboss", factoryMethod);
        }
    }

    std::string OrrukWarboss::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapon") {
            if (parameter.intValue == Boss_Choppa_And_Shield) {
                return "Boss Choppa and Shield";
            } else if (parameter.intValue == Massive_Choppa) {
                return "Massive Choppa";
            } else if (parameter.intValue == Paired_Boss_Choppas) {
                return "Paired Boss Choppas";
            } else if (parameter.intValue == Great_Waaagh_Banner) {
                return "Great Waaagh! Banner";
            }
        }
        return ParameterValueToString(parameter);
    }

    int OrrukWarboss::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Boss Choppa and Shield") {
            return Boss_Choppa_And_Shield;
        } else if (enumString == "Massive Choppa") {
            return Massive_Choppa;
        } else if (enumString == "Paired Boss Choppas") {
            return Paired_Boss_Choppas;
        } else if (enumString == "Great Waaagh! Banner") {
            return Great_Waaagh_Banner;
        }
        return 0;
    }

    int OrrukWarboss::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        int attacks = Unit::extraAttacks(attackingModel, weapon, target);
        // Choppa Boss
        if ((m_weapon == Paired_Boss_Choppas) && (weapon->name() == m_bossChoppa.name())) {
            attacks += 2;
        }
        return attacks;
    }

    Rerolls OrrukWarboss::toSaveRerolls(const Weapon *weapon, const Unit* attacker) const {
        // Boss Shield
        if (m_weapon == Boss_Choppa_And_Shield) {
            return Reroll_Failed;
        }
        return Unit::toSaveRerolls(weapon, attacker);
    }

    int OrrukWarboss::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace Greenskinz
