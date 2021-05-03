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

    static const int g_basesize = 32;
    static const int g_wounds = 2;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 15;
    static const int g_pointsPerBlock = 100;
    static const int g_pointsMaxUnitSize = 300;

    bool AlarithStoneguard::s_registered = false;

    Unit *AlarithStoneguard::Create(const ParameterList &parameters) {
        auto nation = (GreatNation) GetEnumParam("Nation", parameters, (int) GreatNation::None);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto weaponOption = (WeaponOption) GetEnumParam("Weapon", parameters,
                                                        ToInteger(WeaponOption::Diamondpick_Hammer));
        auto standardBearer = GetBoolParam("Standard Bearer", parameters, true);
        return new AlarithStoneguard(nation, numModels, weaponOption, standardBearer, ComputePoints(parameters));
    }

    std::string AlarithStoneguard::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapon") {
            auto name = magic_enum::enum_name((WeaponOption) parameter.intValue);
            return std::string(name);
        }
        return LuminethBase::ValueToString(parameter);
    }

    int AlarithStoneguard::EnumStringToInt(const std::string &enumString) {
        auto weapon = magic_enum::enum_cast<WeaponOption>(enumString);
        if (weapon.has_value()) return (int) weapon.value();

        return LuminethBase::EnumStringToInt(enumString);
    }

    int AlarithStoneguard::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    void AlarithStoneguard::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {ToInteger(WeaponOption::Diamondpick_Hammer),
                                                       ToInteger(WeaponOption::Stone_Mallet)};
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
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

    AlarithStoneguard::AlarithStoneguard(GreatNation nation, int numModels, WeaponOption weaponOption, bool standardBearer, int points) :
            LuminethBase(nation, "Alarith Stoneguard", 4, g_wounds, 7, 4, false, points),
            m_malletOrHammer(Weapon::Type::Melee, "Stone Mallet or Diamondpick Hammer", 1, 2, 3, 3, -1, 1),
            m_stratumHammer(Weapon::Type::Melee, "Stratum Hammer", 1, 3, 3, 4, 0, 1),
            m_pairedStratumHammers(Weapon::Type::Melee, "Paired Stratum Hammers", 1, 3, 3, 4, 0, 1) {
        m_keywords = {ORDER, AELF, LUMINETH_REALM_LORDS, AELEMENTIRI, ALARITH, STONEGUARD};
        m_weapons = {&m_malletOrHammer, &m_stratumHammer, &m_pairedStratumHammers};
        m_battleFieldRole = Role::Battleline;

        m_weaponOption = weaponOption;

        auto seneschal = new Model(g_basesize, wounds());
        seneschal->addMeleeWeapon(&m_pairedStratumHammers);
        seneschal->setName("Truestone Seneschal");
        addModel(seneschal);

        if (standardBearer) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_stratumHammer);
            model->setName(Model::StandardBearer);
            addModel(model);
        }

        int currentModelCount = (int) m_models.size();
        for (auto i = currentModelCount; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_malletOrHammer);
            addModel(model);
        }
    }

    Rerolls AlarithStoneguard::battleshockRerolls() const {
        if (isNamedModelAlive(Model::StandardBearer)) return Rerolls::Failed;
        return Unit::battleshockRerolls();
    }

    Wounds AlarithStoneguard::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        if ((hitRoll == 6) && (weapon->name() == m_malletOrHammer.name())) {
            if (m_weaponOption == WeaponOption::Stone_Mallet) {
                return {weapon->damage() + 1, 0};
            } else if (m_weaponOption == WeaponOption::Diamondpick_Hammer) {
                return {0, 1};
            }
        }
        return Unit::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    Rerolls AlarithStoneguard::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        if (weapon->name() == m_pairedStratumHammers.name()) return Rerolls::Failed;
        return Unit::toHitRerolls(weapon, target);
    }

} // namespace LuminethRealmLords