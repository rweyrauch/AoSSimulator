/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/GraveGuard.h>
#include <UnitFactory.h>
#include "SoulblightGravelordsPrivate.h"

namespace Soulblight {
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 30;
    static const int g_pointsPerBlock = 140;
    static const int g_pointsMaxUnitSize = 3 * g_pointsPerBlock;

    bool GraveGuard::s_registered = false;

    GraveGuard::GraveGuard(CursedBloodline bloodline, int numModels, WeaponOptions weapons, bool standardBearers, bool hornblowers, int points) :
            SoulblightBase(bloodline, "Grave Guard", 4, g_wounds, 10, 5, false, points) {
        m_keywords = {DEATH, SOULBLIGHT_GRAVELORDS, DEATHRATTLE, SUMMONABLE, GRAVE_GUARD};
        m_weapons = {&m_wightBlade, &m_wightBladeSeneschal, &m_greatWightBlade, &m_greatWightBladeSeneschal};
        m_weaponOption = weapons;

        if (standardBearers) {
            m_deathlessMinionsRerolls = Rerolls::Ones;
        }

        auto seneschal = new Model(g_basesize, wounds());
        if (weapons == Wight_Blade) {
            seneschal->addMeleeWeapon(&m_wightBladeSeneschal);
        } else if (weapons == Great_Wight_Blade) {
            seneschal->addMeleeWeapon(&m_greatWightBladeSeneschal);
        }
        addModel(seneschal);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            if (weapons == Wight_Blade) {
                model->addMeleeWeapon(&m_wightBlade);
            } else if (weapons == Great_Wight_Blade) {
                model->addMeleeWeapon(&m_greatWightBlade);
            }
            if (standardBearers) {
                model->setName(Model::StandardBearer);
                standardBearers = false;
            } else if (hornblowers) {
                model->setName(Model::Hornblower);
                hornblowers = false;
            }
            addModel(model);
        }
    }

    GraveGuard::~GraveGuard() {
    }

    Unit *GraveGuard::Create(const ParameterList &parameters) {
        auto bloodline = (CursedBloodline) GetEnumParam("Bloodline", parameters, g_bloodlines[0]);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto weapons = (WeaponOptions) GetEnumParam("Weapons", parameters, Wight_Blade);
        bool standardBearers = GetBoolParam("Standard Bearers", parameters, false);
        bool hornblowers = GetBoolParam("Hornblowers", parameters, false);
        return new GraveGuard(bloodline, numModels, weapons, standardBearers, hornblowers, ComputePoints(parameters));
    }

    void GraveGuard::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Wight_Blade, Great_Wight_Blade};
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Weapons", Wight_Blade, weapons),
                            BoolParameter("Standard Bearers"),
                            BoolParameter("Hornblowers"),
                            EnumParameter("Bloodline", g_bloodlines[0], g_bloodlines)
                    },
                    DEATH,
                    {SOULBLIGHT_GRAVELORDS}
            };
            s_registered = UnitFactory::Register("Grave Guard", factoryMethod);
        }
    }

    std::string GraveGuard::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == Wight_Blade) { return "Wight Blade"; }
            else if (parameter.intValue == Great_Wight_Blade) { return "Great Wight Blade"; }
        }
        return SoulblightBase::ValueToString(parameter);
    }

    int GraveGuard::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Wight Blade") { return Wight_Blade; }
        else if (enumString == "Great Wight Blade") { return Great_Wight_Blade; }
        return SoulblightBase::EnumStringToInt(enumString);
    }

    int GraveGuard::toSaveModifier(const Weapon *weapon, const Unit *attacker) const {
        int modifier = SoulblightBase::toSaveModifier(weapon, attacker);

        // Crypt Shields
        if (m_weaponOption == WeaponOptions::Wight_Blade) {
            modifier += 1;
        }
        return modifier;
    }

    Wounds GraveGuard::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Cursed Weapons
        if (woundRoll >= 6) {
            return {weapon->damage(), 1};
        }
        return SoulblightBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int GraveGuard::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    int GraveGuard::rollChargeDistance() {
        // Hornblower
        auto dist = SoulblightBase::rollChargeDistance();
        if (isNamedModelAlive(Model::Hornblower))
            return std::max(6, dist);
        return dist;
    }

} //namespace Soulblight
