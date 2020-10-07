/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include "lumineth/Stoneguard.h"
#include "../AoSSimPrivate.h"
#include "LuminethPrivate.h"
#include <UnitFactory.h>
#include <magic_enum.hpp>

namespace LuminethRealmLords {

    static const int BASESIZE = 32;
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 15;
    static const int POINTS_PER_BLOCK = 100;
    static const int POINTS_MAX_UNIT_SIZE = 300;

    bool AlarithStoneguard::s_registered = false;

    Unit *AlarithStoneguard::Create(const ParameterList &parameters) {
        auto unit = new AlarithStoneguard();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        auto weaponOption = (WeaponOption)GetEnumParam("Weapon", parameters, to_integer(WeaponOption::Diamondpick_Hammer));
        auto standardBearer = GetBoolParam("Standard Bearer", parameters, true);

        auto nation = (GreatNation)GetEnumParam("Nation", parameters, (int)GreatNation::None);
        unit->setNation(nation);

        bool ok = unit->configure(numModels, weaponOption, standardBearer);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string AlarithStoneguard::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapon") {
            auto name = magic_enum::enum_name((WeaponOption)parameter.intValue);
            return std::string(name);
        }
        return LuminethBase::ValueToString(parameter);
    }

    int AlarithStoneguard::EnumStringToInt(const std::string &enumString) {
        auto weapon = magic_enum::enum_cast<CommandTrait>(enumString);
        if (weapon.has_value()) return (int)weapon.value();

        return LuminethBase::EnumStringToInt(enumString);
    }

    int AlarithStoneguard::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

    void AlarithStoneguard::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {to_integer(WeaponOption::Diamondpick_Hammer), to_integer(WeaponOption::Stone_Mallet)};
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            EnumParameter("Weapon", weapons[0], weapons),
                            BoolParameter("Standard Bearer"),
                            EnumParameter("Nation", g_greatNations[0], g_greatNations),
                    },
                    ORDER,
                    {LUMINETH_REALM_LORDS}
            };

            s_registered = UnitFactory::Register("Alarith Stoneguard", factoryMethod);
        }
    }

    AlarithStoneguard::AlarithStoneguard() :
        LuminethBase("Alarith Stoneguard", 4, WOUNDS, 7, 4, false),
        m_malletOrHammer(Weapon::Type::Melee, "Stone Mallet or Diamondpick Hammer", 1, 2, 3, 3, -1, 1),
        m_stratumHammer(Weapon::Type::Melee, "Stratum Hammer", 1, 3, 3, 4, 0, 1),
        m_pairedStratumHammers(Weapon::Type::Melee, "Paired Stratum Hammers", 1, 3, 3, 4, 0, 1) {
            m_keywords = {ORDER, AELF, LUMINETH_REALM_LORDS, AELEMENTIRI, ALARITH, STONEGUARD};
            m_weapons = {&m_malletOrHammer, &m_stratumHammer, &m_pairedStratumHammers};
            m_battleFieldRole = Role::Battleline;
    }

    bool AlarithStoneguard::configure(int numModels, WeaponOption weaponOption, bool standardBearer) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        m_weaponOption = weaponOption;
        m_standardBearer = standardBearer;

        auto seneschal = new Model(BASESIZE, wounds());
        seneschal->addMeleeWeapon(&m_pairedStratumHammers);
        seneschal->setName("Truestone Seneschal");
        addModel(seneschal);

        if (m_standardBearer) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_stratumHammer);
            model->setName("Standard Bearer");
            addModel(model);
        }

        int currentModelCount = (int) m_models.size();
        for (auto i = currentModelCount; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_malletOrHammer);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Rerolls AlarithStoneguard::battleshockRerolls() const {
        if (m_standardBearer) return RerollFailed;
        return Unit::battleshockRerolls();
    }

    Wounds AlarithStoneguard::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        if ((hitRoll == 6) && (weapon->name() == m_malletOrHammer.name())) {
            if (m_weaponOption == WeaponOption::Stone_Mallet) {
                return {weapon->damage()+1, 0};
            }
            else if (m_weaponOption == WeaponOption::Diamondpick_Hammer) {
                return {0, 1};
            }
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    Rerolls AlarithStoneguard::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        if (weapon->name() == m_pairedStratumHammers.name()) return RerollFailed;
        return Unit::toHitRerolls(weapon, target);
    }

} // namespace LuminethRealmLords