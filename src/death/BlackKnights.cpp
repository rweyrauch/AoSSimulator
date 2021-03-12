/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <death/BlackKnights.h>
#include <UnitFactory.h>
#include "LegionOfNagashPrivate.h"

namespace Death {
    static const int g_basesize = 60; // x35 oval
    static const int g_wounds = 3;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 20;
    static const int g_pointsPerBlock = 120;
    static const int g_pointsMaxUnitSize = 480;

    bool BlackKnights::s_registered = false;

    BlackKnights::BlackKnights() :
            LegionOfNagashBase("Black Knights", 12, g_wounds, 10, 5, false),
            m_barrowLance(Weapon::Type::Melee, "Barrow Lance", 1, 2, 3, 4, 0, 1),
            m_barrowLanceKnight(Weapon::Type::Melee, "Barrow Lance", 1, 3, 3, 4, 0, 1),
            m_hoovesAndTeeth(Weapon::Type::Melee, "Skeletal Steed's Hooves and Teeth", 1, 2, 4, 5, 0, 1) {
        m_keywords = {DEATH, SKELETON, DEATHRATTLE, SUMMONABLE, BLACK_KNIGHTS};
        m_weapons = {&m_barrowLance, &m_barrowLanceKnight, &m_hoovesAndTeeth};
        m_hasMount = true;
        m_hoovesAndTeeth.setMount(true);

        s_globalBraveryMod.connect(this, &BlackKnights::standardBearerBraveryMod, &m_standardSlot);
    }

    BlackKnights::~BlackKnights() {
        m_standardSlot.disconnect();
    }

    bool BlackKnights::configure(int numModels, bool standardBearers, bool hornblowers) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            // Invalid model count.
            return false;
        }

        auto hellKnight = new Model(g_basesize, wounds());
        hellKnight->addMeleeWeapon(&m_barrowLanceKnight);
        hellKnight->addMeleeWeapon(&m_hoovesAndTeeth);
        addModel(hellKnight);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_barrowLance);
            model->addMeleeWeapon(&m_hoovesAndTeeth);
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

    Unit *BlackKnights::Create(const ParameterList &parameters) {
        auto unit = new BlackKnights();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool standardBearers = GetBoolParam("Standard Bearers", parameters, false);
        bool hornblowers = GetBoolParam("Hornblowers", parameters, false);

        auto legion = (Legion) GetEnumParam("Legion", parameters, g_legions[0]);
        unit->setLegion(legion);

        bool ok = unit->configure(numModels, standardBearers, hornblowers);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void BlackKnights::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LegionOfNagashBase::ValueToString,
                    LegionOfNagashBase::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            BoolParameter("Standard Bearers"),
                            BoolParameter("Hornblowers"),
                            EnumParameter("Legion", g_legions[0], g_legions)
                    },
                    DEATH,
                    {DEATHRATTLE}
            };
            s_registered = UnitFactory::Register("Black Knights", factoryMethod);
        }
    }

    int BlackKnights::toWoundModifier(const Weapon *weapon, const Unit *target) const {
        int modifier = Unit::toWoundModifier(weapon, target);

        // Deathly Charge
        if (m_charged && weapon->name() == m_barrowLance.name()) {
            modifier += 1;
        }
        return modifier;
    }

    Wounds BlackKnights::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Deathly Charge
        if (m_charged && weapon->name() == m_barrowLance.name()) {
            return {weapon->damage() + 1, 0};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int BlackKnights::toSaveModifier(const Weapon *weapon, const Unit *attacker) const {
        int modifier = Unit::toSaveModifier(weapon, attacker);

        // Crypt Shields
        if (weapon->rend() == 0) {
            modifier += 1;
        }

        return modifier;
    }

    int BlackKnights::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    int BlackKnights::standardBearerBraveryMod(const Unit *unit) {
        if (isNamedModelAlive(Model::StandardBearer) && !isFriendly(unit) && (distanceTo(unit) <= 6.0)) return -1;
        return 0;
    }

    int BlackKnights::rollChargeDistance() {
        // Hornblower
        auto dist = Unit::rollChargeDistance();
        if (isNamedModelAlive(Model::Hornblower)) {
            return std::max(6, dist);
        }
        return dist;
    }

} //namespace Death
