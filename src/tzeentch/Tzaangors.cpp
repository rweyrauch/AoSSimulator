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

    Tzaangors::Tzaangors() :
            TzeentchBase("Tzaangors", 6, g_wounds, 5, 5, false),
            m_savageBlade(Weapon::Type::Melee, "Savage Blade", 1, 2, 4, 4, 0, 1),
            m_savageBladeTwistbray(Weapon::Type::Melee, "Savage Blade", 1, 2, 3, 4, 0, 1),
            m_savageGreatblade(Weapon::Type::Melee, "Savage Greatblade", 1, 1, 4, 4, -1, 2),
            m_savageGreatbladeTwistbray(Weapon::Type::Melee, "Savage Greatblade", 1, 1, 3, 4, -1, 2),
            m_viciousBeak(Weapon::Type::Melee, "Vicious Beak", 1, 1, 4, 5, 0, 1),
            m_viciousBeakTwistbray(Weapon::Type::Melee, "Vicious Beak", 1, 1, 3, 5, 0, 1) {
        m_keywords = {CHAOS, GOR, TZEENTCH, ARCANITE, TZAANGORS};
        m_weapons = {&m_savageBlade, &m_savageBladeTwistbray, &m_savageGreatblade, &m_savageGreatbladeTwistbray,
                     &m_viciousBeak, &m_viciousBeakTwistbray};
        m_battleFieldRole = Battleline;
    }

    bool Tzaangors::configure(int numModels, WeaponOptions weapons, int numGreatblades, int numMutants, bool iconBearer,
                              bool brayhorns) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }
        const int maxGreatblades = (numGreatblades / g_minUnitSize) * 2;
        if (numGreatblades > maxGreatblades) {
            return false;
        }
        const int maxMutents = numModels / g_minUnitSize;
        if (numMutants > maxMutents) {
            return false;
        }

        m_weaponOption = weapons;
        m_numGreatblades = numGreatblades;
        m_numMutants = numMutants;
        m_iconBearer = iconBearer;
        m_brayhorns = brayhorns;

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
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *Tzaangors::Create(const ParameterList &parameters) {
        auto unit = new Tzaangors();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto weapons = (WeaponOptions) GetEnumParam("Weapons", parameters, Savage_Blade_And_Shield);
        int numGreatblades = GetIntParam("Greatblades", parameters, 0);
        int numMutants = GetIntParam("Mutants", parameters, 0);
        bool iconBearer = GetBoolParam("Icon Bearer", parameters, false);
        bool brayhorns = GetBoolParam("Brayhorns", parameters, false);

        auto coven = (ChangeCoven) GetEnumParam("Change Coven", parameters, (int)ChangeCoven::None);
        unit->setChangeCoven(coven);

        bool ok = unit->configure(numModels, weapons, numGreatblades, numMutants, iconBearer, brayhorns);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
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

    Wounds Tzaangors::applyWoundSave(const Wounds &wounds, Unit* attackingUnit) {
        // Arcanite Shield
        auto totalWounds = Unit::applyWoundSave(wounds, attackingUnit);

        if (m_weaponOption == Savage_Blade_And_Shield) {
            Dice::RollResult normalSaves, mortalSaves;
            Dice::RollD6(totalWounds.normal, normalSaves);
            Dice::RollD6(totalWounds.mortal, mortalSaves);
            totalWounds.normal -= normalSaves.rollsGE(6);
            totalWounds.mortal -= mortalSaves.rollsGE(6);
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

    int Tzaangors::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} //namespace Tzeentch