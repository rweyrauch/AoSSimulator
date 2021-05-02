/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <tzeentch/HorrorsOfTzeentch.h>
#include <UnitFactory.h>
#include <Board.h>
#include "TzeentchPrivate.h"

namespace Tzeentch {
    static const int g_basesize = 32;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 20;
    static const int g_pointsPerBlockPink = 220;
    static const int g_pointsPerBlockBlue = 100;
    static const int g_pointsPerBlockBrimstone = 60;

    bool HorrorsOfTzeentch::s_registered = false;

    HorrorsOfTzeentch::HorrorsOfTzeentch(ChangeCoven coven, int numPink, int numBlue, int numYellow, bool iconBearer, bool hornblower, int points) :
            TzeentchBase("Horrors of Tzeentch", 5, g_wounds, 10, 6, false, points) {
        m_keywords = {CHAOS, DAEMON, TZEENTCH, HORROR, HORROR_OF_TZEENTCH};
        m_weapons = {&m_magicalFlamesPink,
                     &m_magicalFlamesBlue,
                     &m_magicalFlamesBrimstone,
                     &m_talonedHandsPink,
                     &m_talonedHandsBlue,
                     &m_talonedHandsBrimstone,
                     &m_talonedHandsIridescent};
        m_battleFieldRole = (numPink > 0) ? Role::Battleline : Role::Other;
        m_totalUnbinds = 1;
        m_totalSpells = 1;

        setChangeCoven(coven);

        auto horror = new Model(g_basesize, wounds());
        horror->addMissileWeapon(&m_magicalFlamesPink);
        horror->addMeleeWeapon(&m_talonedHandsIridescent);
        horror->setName("Iridescent");
        addModel(horror);

        for (auto i = 1; i < numPink; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_magicalFlamesPink);
            model->addMeleeWeapon(&m_talonedHandsPink);
            model->setName("Pink");
            if (iconBearer) {
                model->setName(Model::IconBearer);
                iconBearer = false;
            } else if (hornblower) {
                model->setName(Model::Hornblower);
                hornblower = false;
            }

            addModel(model);
        }

        for (auto i = 0; i < numBlue; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_magicalFlamesBlue);
            model->addMeleeWeapon(&m_talonedHandsBlue);
            model->setName("Blue");
            addModel(model);
        }

        for (auto i = 0; i < numYellow; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_magicalFlamesBrimstone);
            model->addMeleeWeapon(&m_talonedHandsBrimstone);
            model->setName("Blue");
            addModel(model);
        }
    }

    Unit *HorrorsOfTzeentch::Create(const ParameterList &parameters) {
        int numPink = GetIntParam("Models (Pink)", parameters, g_minUnitSize);
        int numBlue = GetIntParam("Models (Blue)", parameters, 0);
        int numYellow = GetIntParam("Models (Brimstone)", parameters, 0);
        bool iconBearer = GetBoolParam("Icon Bearer", parameters, false);
        bool hornblower = GetBoolParam("Hornblower", parameters, false);
        auto coven = (ChangeCoven) GetEnumParam("Change Coven", parameters, g_changeCoven[0]);

        return new HorrorsOfTzeentch(coven, numPink, numBlue, numYellow, iconBearer, hornblower, ComputePoints(parameters));
    }

    void HorrorsOfTzeentch::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    HorrorsOfTzeentch::Create,
                    TzeentchBase::ValueToString,
                    TzeentchBase::EnumStringToInt,
                    HorrorsOfTzeentch::ComputePoints,
                    {
                            IntegerParameter("Models (Pink)", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            IntegerParameter("Models (Blue)", 0, 0, g_maxUnitSize, g_minUnitSize),
                            IntegerParameter("Models (Brimstone)", 0, 0, g_maxUnitSize, g_minUnitSize),
                            BoolParameter("Icon Bearer"),
                            BoolParameter("Hornblower"),
                            EnumParameter("Change Coven", g_changeCoven[0], g_changeCoven),
                    },
                    CHAOS,
                    {TZEENTCH}
            };
            s_registered = UnitFactory::Register("Horrors of Tzeentch", factoryMethod);
        }
    }

    void HorrorsOfTzeentch::computeBattleshockEffect(int roll, int &numFled, int &numAdded) const {
        TzeentchBase::computeBattleshockEffect(roll, numFled, numAdded);
        if (isNamedModelAlive(Model::IconBearer)) {
            // Icon Bearer
            if (roll == 1) {
                numAdded = Dice::RollD6();
            }
        }
    }

    void HorrorsOfTzeentch::restoreModels(int numModels) {
        // Icon Bearer
        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_magicalFlamesPink);
            model->addMeleeWeapon(&m_talonedHandsPink);
            model->setName("Pink");
            addModel(model);
        }
    }

    int HorrorsOfTzeentch::toHitModifier(const Weapon *weapon, const Unit *target) const {
        int modifier = TzeentchBase::toHitModifier(weapon, target);
        // Flickering Flames
        if (remainingModels() >= 20)
            modifier += 1;
        return modifier;
    }

    int HorrorsOfTzeentch::castingModifier() const {
        int modifier = TzeentchBase::castingModifier();

        // Locus of Conjuration
        auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 9.0);
        for (auto ip : units) {
            if (ip->hasKeyword(TZEENTCH) && ip->hasKeyword(DAEMON) && ip->hasKeyword(HERO)) {
                modifier += 1;
                break;
            }
        }
        return modifier;
    }

    int HorrorsOfTzeentch::ComputePoints(const ParameterList& parameters) {
        int numPink = GetIntParam("Models (Pink)", parameters, g_minUnitSize);
        int numBlue = GetIntParam("Models (Blue)", parameters, 0);
        int numYellow = GetIntParam("Models (Brimstone)", parameters, 0);
        int points = numPink / g_minUnitSize * g_pointsPerBlockPink;
        points += numBlue / g_minUnitSize * g_pointsPerBlockBlue;
        points += numYellow / g_minUnitSize * g_pointsPerBlockBrimstone;
        return points;
    }

    Wounds HorrorsOfTzeentch::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        auto totalWounds = TzeentchBase::applyWoundSave(wounds, attackingUnit);

        // Ectoplasmic Elasticity
        if (isNamedModelAlive("Pink")) {
            return ignoreWounds(totalWounds, 6);
        }
        return totalWounds;
    }

    Rerolls HorrorsOfTzeentch::hornblower(const Unit *unit) {
        if (isNamedModelAlive(Model::Hornblower) && (distanceTo(unit) < 6.0)) {
            return Rerolls::Ones;
        }
        return Rerolls::None;
    }

} //namespace Tzeentch