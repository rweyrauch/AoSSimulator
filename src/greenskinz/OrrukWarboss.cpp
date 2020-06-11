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
    static const int BASESIZE = 32;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 140;

    bool OrrukWarboss::s_registered = false;

    OrrukWarboss::OrrukWarboss() :
            Unit("Orruk Warboss", 5, WOUNDS, 7, 4, false),
            m_bossChoppa(Weapon::Type::Melee, "Boss Choppa", 1, 6, 3, 3, -1, 1),
            m_massiveChoppa(Weapon::Type::Melee, "Massive Choppa", 1, 3, 4, 3, -2, RAND_D3),
            m_greatWaaaghBanner(Weapon::Type::Melee, "Great Waaagh! Banner", 2, 4, 4, 4, 0, 1),
            m_boarTusks(Weapon::Type::Melee, "War Boar's Tusks", 1, 2, 4, 4, 0, 1) {
        m_keywords = {DESTRUCTION, ORRUK, GREENSKINZ, HERO, ORRUK_WARBOSS};
        m_weapons = {&m_bossChoppa, &m_massiveChoppa, &m_greatWaaaghBanner, &m_boarTusks};
    }

    bool OrrukWarboss::configure(WeaponOption weapon, bool warboar) {
        auto model = new Model(BASESIZE, wounds());

        m_weapon = weapon;

        switch (weapon) {
            case BossChoppaAndShield:
            case PairedBossChoppas:
                model->addMeleeWeapon(&m_bossChoppa);
                break;
            case MassiveChoppa:
                model->addMeleeWeapon(&m_massiveChoppa);
                break;
            case GreatWaaaghBanner:
                model->addMeleeWeapon(&m_greatWaaaghBanner);
                addKeyword(TOTEM);
                break;
            default:
                SimLog(Verbosity::Narrative, "Unknown Orruk Warboss weapon %d", weapon);
                break;
        }
        if (warboar) {
            model->addMeleeWeapon(&m_boarTusks);
            m_move = 9;
        }
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *OrrukWarboss::Create(const ParameterList &parameters) {
        auto unit = new OrrukWarboss();
        WeaponOption weapon = (WeaponOption) GetEnumParam("Weapon", parameters, BossChoppaAndShield);
        bool warboar = GetBoolParam("War Boar", parameters, false);

        bool ok = unit->configure(weapon, warboar);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void OrrukWarboss::Init() {
        if (!s_registered) {
            static const std::array<int, 4> weapons = {BossChoppaAndShield, PairedBossChoppas,
                                                       MassiveChoppa, GreatWaaaghBanner};
            static FactoryMethod factoryMethod = {
                    OrrukWarboss::Create,
                    OrrukWarboss::ValueToString,
                    OrrukWarboss::EnumStringToInt,
                    OrrukWarboss::ComputePoints,
                    {
                            EnumParameter("Weapon", BossChoppaAndShield, weapons),
                            BoolParameter("War Boar"),
                    },
                    DESTRUCTION,
                    {GREENSKINZ}
            };
            s_registered = UnitFactory::Register("Orruk Warboss", factoryMethod);
        }
    }

    std::string OrrukWarboss::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapon") {
            if (parameter.intValue == BossChoppaAndShield) {
                return "Boss Choppa and Shield";
            } else if (parameter.intValue == MassiveChoppa) {
                return "Massive Choppa";
            } else if (parameter.intValue == PairedBossChoppas) {
                return "Paired Boss Choppas";
            } else if (parameter.intValue == GreatWaaaghBanner) {
                return "Great Waaagh! Banner";
            }
        }
        return ParameterValueToString(parameter);
    }

    int OrrukWarboss::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Boss Choppa and Shield") {
            return BossChoppaAndShield;
        } else if (enumString == "Massive Choppa") {
            return MassiveChoppa;
        } else if (enumString == "Paired Boss Choppas") {
            return PairedBossChoppas;
        } else if (enumString == "Great Waaagh! Banner") {
            return GreatWaaaghBanner;
        }
        return 0;
    }

    int OrrukWarboss::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        int attacks = Unit::extraAttacks(attackingModel, weapon, target);
        // Choppa Boss
        if ((m_weapon == PairedBossChoppas) && (weapon->name() == m_bossChoppa.name())) {
            attacks += 2;
        }
        return attacks;
    }

    Rerolls OrrukWarboss::toSaveRerolls(const Weapon *weapon) const {
        // Boss Shield
        if (m_weapon == BossChoppaAndShield) {
            return RerollFailed;
        }
        return Unit::toSaveRerolls(weapon);
    }

    int OrrukWarboss::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace Greenskinz
