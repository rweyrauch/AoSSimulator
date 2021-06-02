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
            SoulblightBase(bloodline, "Black Knights", 12, g_wounds, 10, 5, false, points) {
        m_keywords = {DEATH, SOULBLIGHT_GRAVELORDS, DEATHRATTLE, SUMMONABLE, BLACK_KNIGHTS};
        m_weapons = {&m_barrowLance, &m_barrowLanceKnight, &m_hoovesAndTeeth};
        m_hasMount = true;
        m_hoovesAndTeeth.setMount(true);
        m_battleFieldRole = (bloodline == CursedBloodline::Legion_Of_Blood) ? Role::Battleline : Role::Other;

        if (standardBearers) {
            m_deathlessMinionsRerolls = Rerolls::Ones;
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

    int BlackKnights::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    int BlackKnights::rollChargeDistance() {
        // Hornblower
        auto dist = SoulblightBase::rollChargeDistance();
        if (isNamedModelAlive(Model::Hornblower)) {
            return std::max(6, dist);
        }
        return dist;
    }

    void BlackKnights::onCharged() {
        SoulblightBase::onCharged();

        if (m_meleeTarget && (distanceTo(m_meleeTarget) <= 1.0)) {
            if (Dice::RollD6() >= 2) {
                Wounds wounds = {0, Dice::RollD3()};
                m_meleeTarget->applyDamage(wounds, this);

                PLOG_INFO.printf("%s Deathly Charge inflicted %d mortal wounds on %s\n",
                                 name().c_str(), wounds.mortal, m_meleeTarget->name().c_str());
           }
        }
    }

} //namespace Soulblight
