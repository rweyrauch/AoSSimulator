/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/BlackKnights.h>
#include <UnitFactory.h>
#include "SoulblightGravelordsPrivate.h"

namespace Soulblight {
    static const int g_basesize = 60; // x35 oval
    static const int g_wounds = 3;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 15;
    static const int g_pointsPerBlock = 120;
    static const int g_pointsMaxUnitSize = 3 * g_pointsPerBlock;

    bool BlackKnights::s_registered = false;

    BlackKnights::BlackKnights(CursedBloodline bloodline, int numModels, bool standardBearers, bool hornblowers, int points) :
            SoulblightBase(bloodline, "Black Knights", 12, g_wounds, 10, 5, false, points),
            m_barrowLance(Weapon::Type::Melee, "Barrow Lance", 2, 2, 4, 3, 0, 1),
            m_barrowLanceKnight(Weapon::Type::Melee, "Barrow Lance", 2, 3, 4, 3, 0, 1),
            m_hoovesAndTeeth(Weapon::Type::Melee, "Hooves and Teeth", 1, 2, 4, 4, 0, 1) {
        m_keywords = {DEATH, SOULBLIGHT_GRAVELORDS, DEATHRATTLE, SUMMONABLE, BLACK_KNIGHTS};
        m_weapons = {&m_barrowLance, &m_barrowLanceKnight, &m_hoovesAndTeeth};
        m_hasMount = true;
        m_hoovesAndTeeth.setMount(true);
        m_battleFieldRole = (bloodline == CursedBloodline::Legion_Of_Blood) ? Role::Battleline : Role::Other;

        s_globalBraveryMod.connect(this, &BlackKnights::standardBearerBraveryMod, &m_standardSlot);

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
    }

    BlackKnights::~BlackKnights() {
        m_standardSlot.disconnect();
    }

    Unit *BlackKnights::Create(const ParameterList &parameters) {
        auto bloodline = (CursedBloodline) GetEnumParam("Bloodline", parameters, g_bloodlines[0]);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool standardBearers = GetBoolParam("Standard Bearers", parameters, false);
        bool hornblowers = GetBoolParam("Hornblowers", parameters, false);
        return new BlackKnights(bloodline, numModels, standardBearers, hornblowers, ComputePoints(parameters));
    }

    void BlackKnights::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SoulblightBase::ValueToString,
                    SoulblightBase::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            BoolParameter("Standard Bearers"),
                            BoolParameter("Hornblowers"),
                            EnumParameter("Bloodline", g_bloodlines[0], g_bloodlines)
                    },
                    DEATH,
                    {SOULBLIGHT_GRAVELORDS}
            };
            s_registered = UnitFactory::Register("Black Knights", factoryMethod);
        }
    }

    int BlackKnights::toWoundModifier(const Weapon *weapon, const Unit *target) const {
        int modifier = SoulblightBase::toWoundModifier(weapon, target);

        // Deathly Charge
        if (m_charged && weapon->name() == m_barrowLance.name()) {
            modifier += 1;
        }
        return modifier;
    }

    Wounds BlackKnights::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Deathly Charge
        if (m_charged && weapon->name() == m_barrowLance.name()) {
            return {weapon->damage() + 1, 0};
        }
        return SoulblightBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int BlackKnights::toSaveModifier(const Weapon *weapon, const Unit *attacker) const {
        int modifier = SoulblightBase::toSaveModifier(weapon, attacker);

        // Crypt Shields
        if (weapon->rend() == 0) {
            modifier += 1;
        }

        return modifier;
    }

    int BlackKnights::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
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
        auto dist = SoulblightBase::rollChargeDistance();
        if (isNamedModelAlive(Model::Hornblower)) {
            return std::max(6, dist);
        }
        return dist;
    }

} //namespace Soulblight