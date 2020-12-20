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
    static const int g_pointsPerBlock = 220;
    static const int g_pointsMaxUnitSize = 440;

    bool HorrorsOfTzeentch::s_registered = false;

    HorrorsOfTzeentch::HorrorsOfTzeentch() :
            TzeentchBase("Horrors of Tzeentch", 5, g_wounds, 10, 6, false),
            m_magicalFlamesPink(Weapon::Type::Missile, "Magical Flames (Pink)", 12, 3, 5, 4, 0, 1),
            m_magicalFlamesBlue(Weapon::Type::Missile, "Magical Flames (Blue)", 12, 2, 5, 4, 0, 1),
            m_magicalFlamesBrimstone(Weapon::Type::Missile, "Magical Flames (Brimstone)", 12, 1, 5, 4, 0, 1),
            m_talonedHandsPink(Weapon::Type::Melee, "Taloned Hands (Pink)", 1, 1, 5, 4, 0, 1),
            m_talonedHandsBlue(Weapon::Type::Melee, "Taloned Hands (Blue)", 1, 1, 5, 4, 0, 1),
            m_talonedHandsBrimstone(Weapon::Type::Melee, "Taloned Hands (Brimstone)", 1, 2, 5, 4, 0, 1),
            m_talonedHandsIridescent(Weapon::Type::Melee, "Taloned Hands (Iridescent)", 1, 2, 5, 4, 0, 1) {
        m_keywords = {CHAOS, DAEMON, TZEENTCH, HORROR, HORROR_OF_TZEENTCH};
        m_weapons = {&m_magicalFlamesPink,
                     &m_magicalFlamesBlue,
                     &m_magicalFlamesBrimstone,
                     &m_talonedHandsPink,
                     &m_talonedHandsBlue,
                     &m_talonedHandsBrimstone,
                     &m_talonedHandsIridescent};
        m_battleFieldRole = Battleline;
        m_totalUnbinds = 1;
        m_totalSpells = 1;
    }

    bool HorrorsOfTzeentch::configure(int numModels, bool iconBearer, bool hornblower) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        auto horror = new Model(g_basesize, wounds());
        horror->addMissileWeapon(&m_magicalFlamesPink);
        horror->addMeleeWeapon(&m_talonedHandsIridescent);
        addModel(horror);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_magicalFlamesPink);
            model->addMeleeWeapon(&m_talonedHandsPink);
            if (iconBearer) {
                model->setName(Model::IconBearer);
                iconBearer = false;
            }
            else if (hornblower) {
                model->setName(Model::Hornblower);
                hornblower = false;
            }

            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *HorrorsOfTzeentch::Create(const ParameterList &parameters) {
        auto unit = new HorrorsOfTzeentch();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool iconBearer = GetBoolParam("Icon Bearer", parameters, false);
        bool hornblower = GetBoolParam("Hornblower", parameters, false);

        auto coven = (ChangeCoven) GetEnumParam("Change Coven", parameters, g_changeCoven[0]);
        unit->setChangeCoven(coven);

        bool ok = unit->configure(numModels, iconBearer, hornblower);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void HorrorsOfTzeentch::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    HorrorsOfTzeentch::Create,
                    TzeentchBase::ValueToString,
                    TzeentchBase::EnumStringToInt,
                    HorrorsOfTzeentch::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
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

    int HorrorsOfTzeentch::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    Wounds HorrorsOfTzeentch::applyWoundSave(const Wounds &wounds, Unit* attackingUnit) {
        auto totalWounds = TzeentchBase::applyWoundSave(wounds, attackingUnit);

        // Ectoplasmic Elasticity
        // TODO: only applies when the unit has Pink Horrors
        Dice::RollResult resultNormal, resultMortal;

        Dice::RollD6(wounds.normal, resultNormal);
        Dice::RollD6(wounds.mortal, resultMortal);

        Wounds negatedWounds = {resultNormal.rollsGE(6), resultNormal.rollsGE(6)};
        totalWounds -= negatedWounds;
        return totalWounds.clamp();
    }

} //namespace Tzeentch