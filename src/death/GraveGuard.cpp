/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <death/GraveGuard.h>
#include <UnitFactory.h>
#include "LegionOfNagashPrivate.h"

namespace Death {
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 30;
    static const int g_pointsPerBlock = 140;
    static const int g_pointsMaxUnitSize = 360;

    bool GraveGuard::s_registered = false;

    GraveGuard::GraveGuard() :
            LegionOfNagashBase("Grave Guard", 4, g_wounds, 10, 5, false),
            m_wightBlade(Weapon::Type::Melee, "Wight Blade", 1, 2, 3, 4, -1, 1),
            m_wightBladeSeneschal(Weapon::Type::Melee, "Wight Blade", 1, 3, 3, 4, -1, 1),
            m_greatWightBlade(Weapon::Type::Melee, "Great Wight Blade", 1, 2, 3, 3, -1, 1),
            m_greatWightBladeSeneschal(Weapon::Type::Melee, "Great Wight Blade", 1, 3, 3, 3, -1, 1) {
        m_keywords = {DEATH, SKELETON, DEATHRATTLE, SUMMONABLE, GRAVE_GUARD};
        m_weapons = {&m_wightBlade, &m_wightBladeSeneschal, &m_greatWightBlade, &m_greatWightBladeSeneschal};
        s_globalBraveryMod.connect(this, &GraveGuard::standardBearerBraveryMod, &m_standardSlot);
    }

    GraveGuard::~GraveGuard() {
        m_standardSlot.disconnect();
    }

    bool GraveGuard::configure(int numModels, GraveGuard::WeaponOptions weapons,
                               bool standardBearers, bool hornblowers) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            // Invalid model count.
            return false;
        }

        m_weaponOption = weapons;

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

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *GraveGuard::Create(const ParameterList &parameters) {
        auto unit = new GraveGuard();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        WeaponOptions weapons = (WeaponOptions) GetEnumParam("Weapons", parameters, Wight_Blade);
        bool standardBearers = GetBoolParam("Standard Bearers", parameters, false);
        bool hornblowers = GetBoolParam("Hornblowers", parameters, false);

        auto legion = (Legion) GetEnumParam("Legion", parameters, g_legions[0]);
        unit->setLegion(legion);

        bool ok = unit->configure(numModels, weapons, standardBearers, hornblowers);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
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
                            EnumParameter("Legion", g_legions[0], g_legions)
                    },
                    DEATH,
                    {DEATHRATTLE}
            };
            s_registered = UnitFactory::Register("Grave Guard", factoryMethod);
        }
    }

    std::string GraveGuard::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == Wight_Blade) { return "Wight Blade"; }
            else if (parameter.intValue == Great_Wight_Blade) { return "Great Wight Blade"; }
        }
        return LegionOfNagashBase::ValueToString(parameter);
    }

    int GraveGuard::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Wight Blade") { return Wight_Blade; }
        else if (enumString == "Great Wight Blade") { return Great_Wight_Blade; }
        return LegionOfNagashBase::EnumStringToInt(enumString);
    }

    int GraveGuard::toSaveModifier(const Weapon *weapon, const Unit *attacker) const {
        int modifier = Unit::toSaveModifier(weapon, attacker);

        // Crypt Shields
        if (weapon->rend() == 0) {
            modifier += 1;
        }

        return modifier;
    }

    Wounds GraveGuard::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        if (m_charged && (woundRoll >= 6)) {
            // Cursed Weapons 2x damage on 6+
            return {weapon->damage() + weapon->damage(), 0};
        }
        return LegionOfNagashBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int GraveGuard::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    int GraveGuard::rollChargeDistance() {
        // Hornblower
        auto dist = LegionOfNagashBase::rollChargeDistance();
        if (isNamedModelAlive(Model::Hornblower))
            return std::max(6, dist);
        return dist;
    }

    int GraveGuard::standardBearerBraveryMod(const Unit *unit) {
        if (isNamedModelAlive(Model::StandardBearer) && !isFriendly(unit) && (distanceTo(unit) <= 6.0)) return -1;
        return 0;
    }

} //namespace Death
