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
    static const int BASESIZE = 60; // x35
    static const int WOUNDS = 3;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 15;
    static const int POINTS_PER_BLOCK = 180;
    static const int POINTS_MAX_UNIT_SIZE = 480;

    bool KavalosDeathriders::s_registered = false;

    Unit *KavalosDeathriders::Create(const ParameterList &parameters) {
        auto unit = new KavalosDeathriders();

        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        auto weapons = (WeaponOption) GetEnumParam("Weapons", parameters, NadiriteBladeAndShield);
        bool necrophoros = GetBoolParam("Necrophoros", parameters, true);

        auto legion = (Legion) GetEnumParam("Legion", parameters, g_legion[0]);
        unit->setLegion(legion);

        bool ok = unit->configure(numModels, weapons, necrophoros);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string KavalosDeathriders::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == NadirateSpearAndShield) {
                return "Nadirate Spear and Shield";
            } else if (parameter.intValue == NadiriteBladeAndShield) {
                return "Nadirite Blade and Shield";
            }
        }
        return OssiarchBonereaperBase::ValueToString(parameter);
    }

    int KavalosDeathriders::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Nadirite Blade and Shield") return NadiriteBladeAndShield;
        else if (enumString == "Nadirate Spear and Shield") return NadirateSpearAndShield;

        return OssiarchBonereaperBase::EnumStringToInt(enumString);
    }

    void KavalosDeathriders::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {NadiriteBladeAndShield, NadirateSpearAndShield};
            static FactoryMethod factoryMethod = {
                    KavalosDeathriders::Create,
                    KavalosDeathriders::ValueToString,
                    KavalosDeathriders::EnumStringToInt,
                    KavalosDeathriders::ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            BoolParameter("Necrophoros"),
                            EnumParameter("Weapons", NadiriteBladeAndShield, weapons),
                            EnumParameter("Legion", g_legion[0], g_legion),
                    },
                    DEATH,
                    {OSSIARCH_BONEREAPERS}
            };
            s_registered = UnitFactory::Register("Kavalos Deathriders", factoryMethod);
        }
    }

    KavalosDeathriders::KavalosDeathriders() :
            OssiarchBonereaperBase("Kavalos Deathriders", 12, WOUNDS, 10, 4, false),
            m_blade(Weapon::Type::Melee, "Nadirite Blade", 1, 3, 3, 4, -1, 1),
            m_spear(Weapon::Type::Melee, "Nadirite Spear", 2, 3, 3, 4, 0, 1),
            m_bladeHekatos(Weapon::Type::Melee, "Nadirite Blade", 1, 4, 3, 4, -1, 1),
            m_spearHekatos(Weapon::Type::Melee, "Nadirite Spear", 2, 4, 3, 4, 0, 1),
            m_hoovesAndTeeth(Weapon::Type::Melee, "Hooves and Teeth", 1, 2, 4, 3, 0, 1) {
        m_keywords = {DEATH, OSSIARCH_BONEREAPERS, KAVALOS_DEATHRIDERS};
        m_weapons = {&m_blade, &m_spear, &m_bladeHekatos, &m_spearHekatos, &m_hoovesAndTeeth};
    }

    bool KavalosDeathriders::configure(int numModels, WeaponOption option, bool necrophoros) {
        // validate inputs
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            // Invalid model count.
            return false;
        }

        auto hekatos = new Model(BASESIZE, wounds());
        if (option == NadiriteBladeAndShield) {
            hekatos->addMeleeWeapon(&m_bladeHekatos);
        } else if (option == NadirateSpearAndShield) {
            hekatos->addMeleeWeapon(&m_spearHekatos);
        }
        hekatos->addMeleeWeapon(&m_hoovesAndTeeth);
        addModel(hekatos);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            if (option == NadiriteBladeAndShield) {
                model->addMeleeWeapon(&m_bladeHekatos);
            } else if (option == NadirateSpearAndShield) {
                model->addMeleeWeapon(&m_spearHekatos);
            }
            model->addMeleeWeapon(&m_hoovesAndTeeth);
            addModel(model);
        }

        m_necrophoros = necrophoros;

        m_points = ComputePoints(numModels);

        return true;
    }

    int KavalosDeathriders::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        if (m_charged && (weapon->name() == m_spear.name()) && (unmodifiedHitRoll >= 5)) return 2;
        if ((unmodifiedHitRoll == 6) &&
            ((weapon->name() == m_blade.name()) || (weapon->name() == m_spear.name())))
            return 2;
        return Unit::generateHits(unmodifiedHitRoll, weapon, unit);
    }

    int KavalosDeathriders::runModifier() const {
        auto mod = Unit::runModifier();
        if (m_necrophoros) mod++;
        return mod;
    }

    int KavalosDeathriders::chargeModifier() const {
        auto mod = Unit::chargeModifier();
        if (m_necrophoros) mod++;
        return mod;
    }

    int KavalosDeathriders::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace OssiarchBonereapers
