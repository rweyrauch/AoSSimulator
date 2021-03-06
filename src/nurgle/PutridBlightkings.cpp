/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <nurgle/PutridBlightkings.h>
#include <UnitFactory.h>
#include <Board.h>
#include "NurglePrivate.h"

namespace Nurgle {
    static const int g_basesize = 40;
    static const int g_wounds = 4;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 20;
    static const int g_pointsPerBlock = 140;
    static const int g_pointsMaxUnitSize = 500;

    bool PutridBlightkings::s_registered = false;

    PutridBlightkings::PutridBlightkings(PlagueLegion legion, int numModels, bool iconBearer, bool sonorousTocsin, int points) :
            NurgleBase(legion,"Putrid Blightkings", 4, g_wounds, 8, 4, false, points),
            m_blightedWeapon(Weapon::Type::Melee, "Blighted Weapon", 1, 3, 3, 3, 0, 1) {
        m_keywords = {CHAOS, MORTAL, NURGLE, ROTBRINGER, PUTRID_BLIGHTKINGS};
        m_weapons = {&m_blightedWeapon};

        m_sonorousTocsin = sonorousTocsin;

        // Add the Blightlord
        auto leader = new Model(g_basesize, g_wounds + 1);
        leader->addMeleeWeapon(&m_blightedWeapon);
        addModel(leader);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_blightedWeapon);
            if (iconBearer) {
                model->setName(Model::IconBearer);
                iconBearer = false;
            }
            addModel(model);
        }
    }

    Unit *PutridBlightkings::Create(const ParameterList &parameters) {
        auto legion = (PlagueLegion) GetEnumParam("Plague Legion", parameters, (int) PlagueLegion::None);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool iconBearer = GetBoolParam("Icon Bearer", parameters, false);
        bool sonorousTocsin = GetBoolParam("Sonorous Tocsin", parameters, false);
        return new PutridBlightkings(legion, numModels, iconBearer, sonorousTocsin, ComputePoints(parameters));
    }

    void PutridBlightkings::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    PutridBlightkings::Create,
                    NurgleBase::ValueToString,
                    NurgleBase::EnumStringToInt,
                    PutridBlightkings::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            BoolParameter("Icon Bearer"),
                            BoolParameter("Sonorous Tocsin"),
                            EnumParameter("Plague Legion", g_plagueLegions[0], g_plagueLegions),
                    },
                    CHAOS,
                    {NURGLE}
            };
            s_registered = UnitFactory::Register("Putrid Blightkings", factoryMethod);
        }
    }

    int PutridBlightkings::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        // Blighted Weapons
        if (unmodifiedHitRoll == 6) {
            // each 6 inflicts D6 hits
            return Dice::RollD6();
        }

        return NurgleBase::generateHits(unmodifiedHitRoll, weapon, unit);
    }

    int PutridBlightkings::runModifier() const {
        int modifier = NurgleBase::runModifier();
        if (m_sonorousTocsin) {
            modifier += 1;
        }
        return modifier;
    }

    int PutridBlightkings::chargeModifier() const {
        int modifier = NurgleBase::chargeModifier();
        if (m_sonorousTocsin) {
            modifier += 1;
        }
        return modifier;
    }

    int PutridBlightkings::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    void PutridBlightkings::onStartHero(PlayerId player) {
        NurgleBase::onStartHero(player);

        if (owningPlayer() == player) {
            auto units = Board::Instance()->getUnitsWithin(this, PlayerId::None, 3.0);

            // Virulent Discharge
            for (auto unit : units) {
                if (Dice::RollD6() >= 6) {
                    if (unit->hasKeyword(NURGLE)) unit->heal(Dice::RollD3());
                    else unit->applyDamage({0, Dice::RollD3()}, this);
                }
            }
        }

    }

} // namespace Nurgle

