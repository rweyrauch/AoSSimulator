/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <dispossessed/Longbeards.h>
#include <UnitFactory.h>
#include "DispossessedPrivate.h"

namespace Dispossessed {
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 30;
    static const int g_pointsPerBlock = 100;
    static const int g_pointsMaxUnitSize = 270;

    bool Longbeards::s_registered = false;

    Longbeards::Longbeards() :
            Dispossessed("Longbeards", 4, g_wounds, 7, 4, false),
            m_ancestralAxeHammer(Weapon::Type::Melee, "Ancestral Axe or Ancestral Hammer", 1, 1, 3, 4, 0, 1),
            m_ancestralGreatAxe(Weapon::Type::Melee, "Ancestral Great Axe", 1, 1, 4, 3, -1, 1),
            m_ancestralAxeHammerOldGuard(Weapon::Type::Melee, "Ancestral Axe or Ancestral Hammer", 1, 2, 3, 4, 0, 1),
            m_ancestralGreatAxeOldGuard(Weapon::Type::Melee, "Ancestral Great Axe", 1, 2, 4, 3, -1, 1) {
        m_keywords = {ORDER, DUARDIN, DISPOSSESSED, LONGBEARDS};
        m_weapons = {&m_ancestralAxeHammer, &m_ancestralGreatAxe, &m_ancestralAxeHammerOldGuard,
                     &m_ancestralGreatAxeOldGuard};
    }

    bool Longbeards::configure(int numModels, WeaponOptions weapons, bool gromrilShields, bool standardBearer,
                               bool musician) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        m_gromrilShields = gromrilShields;

        auto oldguard = new Model(g_basesize, wounds());
        if (weapons == Ancestral_Axes_Or_Hammers) {
            oldguard->addMeleeWeapon(&m_ancestralAxeHammerOldGuard);
        } else if (weapons == Ancestral_Great_Axe) {
            oldguard->addMeleeWeapon(&m_ancestralGreatAxeOldGuard);
        }
        addModel(oldguard);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            if (weapons == Ancestral_Axes_Or_Hammers) {
                model->addMeleeWeapon(&m_ancestralAxeHammer);
            } else if (weapons == Ancestral_Great_Axe) {
                model->addMeleeWeapon(&m_ancestralGreatAxe);
            }
            if (standardBearer) {
                model->setName(Model::StandardBearer);
                standardBearer = false;
            }
            else if (musician) {
                model->setName(Model::Musician);
                musician = false;
            }
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *Longbeards::Create(const ParameterList &parameters) {
        auto unit = new Longbeards();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto weapons = (WeaponOptions) GetEnumParam("Weapons", parameters, Ancestral_Axes_Or_Hammers);
        bool gromrilShields = GetBoolParam("Gromril Shields", parameters, false);
        bool standardBearer = GetBoolParam("Standard Bearer", parameters, false);
        bool musician = GetBoolParam("Musician", parameters, false);

        bool ok = unit->configure(numModels, weapons, gromrilShields, standardBearer, musician);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Longbeards::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Ancestral_Axes_Or_Hammers, Ancestral_Great_Axe};
            static FactoryMethod factoryMethod = {
                    Longbeards::Create,
                    Longbeards::ValueToString,
                    Longbeards::EnumStringToInt,
                    Longbeards::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Weapons", Ancestral_Axes_Or_Hammers, weapons),
                            BoolParameter("Gromril Shields"),
                            BoolParameter("Standard Bearer"),
                            BoolParameter("Musician"),
                            EnumParameter("Grudge", g_grudge[0], g_grudge)
                    },
                    ORDER,
                    {DISPOSSESSED}
            };
            s_registered = UnitFactory::Register("Longbeards", factoryMethod);
        }
    }

    Rerolls Longbeards::toSaveRerolls(const Weapon *weapon, const Unit* attacker) const {
        // Gromril Shields
        if (!m_ran && !m_charged) {
            if (!weapon->isMissile())
                return Rerolls::Failed;
        }
        return Dispossessed::toSaveRerolls(weapon, attacker);
    }

    std::string Longbeards::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == Ancestral_Axes_Or_Hammers) {
                return "Ancestral Axes Or Hammers";
            } else if (parameter.intValue == Ancestral_Great_Axe) {
                return "Ancestral Great Axe";
            }
        }

        return Dispossessed::ValueToString(parameter);
    }

    int Longbeards::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Ancestral Axes Or Hammers") {
            return Ancestral_Axes_Or_Hammers;
        } else if (enumString == "Ancestral Great Axe") {
            return Ancestral_Great_Axe;
        }
        return Dispossessed::EnumStringToInt(enumString);
    }

    int Longbeards::rollRunDistance() {
        // Sound the Advance
        if (isNamedModelAlive(Model::Musician)) {
            return 4;
        }
        return Unit::rollRunDistance();
    }

    void Longbeards::computeBattleshockEffect(int roll, int &numFled, int &numAdded) const {
        Dispossessed::computeBattleshockEffect(roll, numFled, numAdded);
        if (isNamedModelAlive(Model::StandardBearer)) {
            numFled = (numFled + 1) / 2;
        }
    }

    int Longbeards::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace Dispossessed
