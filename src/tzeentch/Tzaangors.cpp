/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019-20 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <tzeentch/Tzaangors.h>
#include <UnitFactory.h>
#include <Board.h>
#include "TzeentchPrivate.h"

namespace Tzeentch {
    static const int g_basesize = 32;
    static const int g_wounds = 2;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 30;
    static const int g_pointsPerBlock = 180;
    static const int g_pointsMaxUnitSize = g_pointsPerBlock * 3;

    bool Tzaangors::s_registered = false;

    Tzaangors::Tzaangors(ChangeCoven coven, int numModels, WeaponOptions weapons, int numGreatblades, int numMutants, bool iconBearer,
                         bool brayhorns, int points) :
            TzeentchBase(coven, "Tzaangors", 6, g_wounds, 5, 5, false, points) {
        m_keywords = {CHAOS, GOR, TZEENTCH, ARCANITE, TZAANGORS};
        m_weapons = {&m_savageBlade, &m_savageBladeTwistbray, &m_savageGreatblade, &m_savageGreatbladeTwistbray,
                     &m_viciousBeak, &m_viciousBeakTwistbray};
        m_battleFieldRole = Role::Battleline;

        setChangeCoven(coven);

        m_weaponOption = weapons;
        m_numGreatblades = numGreatblades;
        m_numMutants = numMutants;

        // Brayhorns
        m_runAndCharge = brayhorns;

        auto twistbray = new Model(g_basesize, wounds());
        if (numGreatblades)
            twistbray->addMeleeWeapon(&m_savageGreatbladeTwistbray);
        else
            twistbray->addMeleeWeapon(&m_savageBladeTwistbray);
        twistbray->addMeleeWeapon(&m_viciousBeakTwistbray);
        addModel(twistbray);

        // the Twistbray is always given a greatblade if there are any greatblades
        for (auto i = 1; i < numGreatblades; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_savageGreatblade);
            model->addMeleeWeapon(&m_viciousBeak);
            addModel(model);
        }

        int currentModelCount = (int) m_models.size();
        for (auto i = currentModelCount; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_savageBlade);
            model->addMeleeWeapon(&m_viciousBeak);
            if (iconBearer) {
                model->setName(Model::IconBearer);
                iconBearer = false;
            } else if (brayhorns) {
                model->setName("Brayhorn");
                brayhorns = false;
            }
            addModel(model);
        }
    }

    Unit *Tzaangors::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto weapons = (WeaponOptions) GetEnumParam("Weapons", parameters, Savage_Blade_And_Shield);
        int numGreatblades = GetIntParam("Greatblades", parameters, 0);
        int numMutants = GetIntParam("Mutants", parameters, 0);
        bool iconBearer = GetBoolParam("Icon Bearer", parameters, false);
        bool brayhorns = GetBoolParam("Brayhorns", parameters, false);
        auto coven = (ChangeCoven) GetEnumParam("Change Coven", parameters, (int) ChangeCoven::None);

        return new Tzaangors(coven, numModels, weapons, numGreatblades, numMutants, iconBearer, brayhorns, ComputePoints(parameters));
    }

    void Tzaangors::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Paired_Savage_Blades, Savage_Blade_And_Shield};
            static FactoryMethod factoryMethod = {
                    Tzaangors::Create,
                    Tzaangors::ValueToString,
                    Tzaangors::EnumStringToInt,
                    Tzaangors::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Weapons", Paired_Savage_Blades, weapons),
                            IntegerParameter("Greatblades", 0, 0, g_maxUnitSize / g_minUnitSize * 2, 1),
                            IntegerParameter("Mutants", 0, 0, g_maxUnitSize / g_minUnitSize, 1),
                            BoolParameter("Icon Bearer"),
                            BoolParameter("Brayhorns"),
                            EnumParameter("Change Coven", g_changeCoven[0], g_changeCoven),
                    },
                    CHAOS,
                    {TZEENTCH, BEASTS_OF_CHAOS}
            };
            s_registered = UnitFactory::Register("Tzaangors", factoryMethod);
        }
    }

    std::string Tzaangors::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == Savage_Blade_And_Shield) {
                return "Savage Blade And Shield";
            } else if (parameter.intValue == Paired_Savage_Blades) {
                return "Paired Savage Blades";
            }
        }

        return TzeentchBase::ValueToString(parameter);
    }

    int Tzaangors::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Savage Blade And Shield") {
            return Savage_Blade_And_Shield;
        } else if (enumString == "Paired Savage Blades") {
            return Paired_Savage_Blades;
        }
        return TzeentchBase::EnumStringToInt(enumString);
    }

    Wounds Tzaangors::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        // Arcanite Shield
        auto totalWounds = TzeentchBase::applyWoundSave(wounds, attackingUnit);

        if (m_weaponOption == Savage_Blade_And_Shield) {
            int numSixes = 0;
            totalWounds = ignoreWounds(totalWounds, 6, numSixes);
        }
        return totalWounds;
    }

    int Tzaangors::toHitModifier(const Weapon *weapon, const Unit *target) const {
        int modifier = TzeentchBase::toHitModifier(weapon, target);

        // Paired Savage Blades
        if ((m_weaponOption == Paired_Savage_Blades) && (weapon->name() == m_savageBlade.name()))
            modifier += 1;

        return modifier;
    }

    int Tzaangors::toWoundModifier(const Weapon *weapon, const Unit *target) const {
        int modifier = TzeentchBase::toWoundModifier(weapon, target);

        // Destined Mayhem
        auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 12.0);
        for (auto ip : units) {
            if (ip->hasKeyword(ARCANITE) && ip->hasKeyword(HERO)) {
                modifier += 1;
                break;
            }
        }

        return modifier;
    }

    int Tzaangors::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        int attacks = TzeentchBase::extraAttacks(attackingModel, weapon, target);

        // Savagery Unleashed
        if (remainingModels() >= 9) attacks++;

        return attacks;
    }

    int Tzaangors::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    void Tzaangors::onStartHero(PlayerId player) {
        TzeentchBase::onStartHero(player);

        // Ornate Totems
        if (owningPlayer() == player) {
            if (isNamedModelAlive(Model::IconBearer)) {
                auto enemy = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
                if (enemy && (distanceTo(enemy) < 18.0)) {
                    auto wizards = Board::Instance()->getUnitsWithKeyword(PlayerId::None, WIZARD);
                    int totalWizards = 0;
                    for (auto wizard : wizards) {
                        if ((wizard->remainingModels() > 0) && (distanceTo(wizard) < 9.0)) {
                            totalWizards++;
                        }
                    }
                    Dice::RollResult rolls;
                    Dice::RollD6(totalWizards, rolls);
                    enemy->applyDamage({0, rolls.rollsGE(4), Wounds::Source::Ability}, this);
                }
            }
        }
    }

} //namespace Tzeentch