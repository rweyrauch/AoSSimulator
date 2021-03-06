/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <citiesofsigmar/Longbeards.h>
#include <UnitFactory.h>
#include "CitiesOfSigmarPrivate.h"

namespace CitiesOfSigmar {
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 30;
    static const int g_pointsPerBlock = 100;
    static const int g_pointsMaxUnitSize = 240;

    bool Longbeards::s_registered = false;

    Longbeards::Longbeards(City city, int numModels, WeaponOptions weapons, bool standardBearer, bool musician, int points) :
            CitizenOfSigmar(city, "Longbeards", 4, g_wounds, 7, 4, false, points),
            m_ancestralAxeHammer(Weapon::Type::Melee, "Ancestral Axe or Ancestral Hammer", 1, 1, 3, 4, 0, 1),
            m_ancestralGreatAxe(Weapon::Type::Melee, "Ancestral Great Axe", 1, 1, 4, 3, -1, 1),
            m_ancestralAxeHammerOldGuard(Weapon::Type::Melee, "Ancestral Axe or Ancestral Hammer", 1, 2, 3, 4, 0, 1),
            m_ancestralGreatAxeOldGuard(Weapon::Type::Melee, "Ancestral Great Axe", 1, 2, 4, 3, -1, 1) {
        m_keywords = {ORDER, DUARDIN, DISPOSSESSED, LONGBEARDS};
        m_weapons = {&m_ancestralAxeHammer, &m_ancestralGreatAxe, &m_ancestralAxeHammerOldGuard,
                     &m_ancestralGreatAxeOldGuard};
        m_battleFieldRole = Role::Battleline;

        auto oldguard = new Model(g_basesize, wounds());
        if (weapons == Ancestral_Weapon_And_Shield) {
            oldguard->addMeleeWeapon(&m_ancestralAxeHammerOldGuard);
        } else if (weapons == Ancestral_Great_Axe) {
            oldguard->addMeleeWeapon(&m_ancestralGreatAxeOldGuard);
        }
        addModel(oldguard);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            if (weapons == Ancestral_Weapon_And_Shield) {
                model->addMeleeWeapon(&m_ancestralAxeHammer);
            } else if (weapons == Ancestral_Great_Axe) {
                model->addMeleeWeapon(&m_ancestralGreatAxe);
            }
            if (standardBearer) {
                model->setName(Model::StandardBearer);
                standardBearer = false;
            } else if (musician) {
                model->setName(Model::Musician);
                musician = false;
            }
            addModel(model);
        }

        m_weaponOption = weapons;
    }

    Unit *Longbeards::Create(const ParameterList &parameters) {
        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto weapons = (WeaponOptions) GetEnumParam("Weapons", parameters, Ancestral_Weapon_And_Shield);
        bool standardBearer = GetBoolParam("Standard Bearer", parameters, false);
        bool musician = GetBoolParam("Musician", parameters, false);
        return new Longbeards(city, numModels, weapons, standardBearer, musician, ComputePoints(parameters));
    }

    void Longbeards::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Ancestral_Weapon_And_Shield, Ancestral_Great_Axe};
            static FactoryMethod factoryMethod = {
                    Longbeards::Create,
                    Longbeards::ValueToString,
                    Longbeards::EnumStringToInt,
                    Longbeards::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Weapons", Ancestral_Weapon_And_Shield, weapons),
                            BoolParameter("Standard Bearer"),
                            BoolParameter("Musician"),
                            EnumParameter("City", g_city[0], g_city),
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Longbeards", factoryMethod);
        }
    }

    Rerolls Longbeards::toSaveRerolls(const Weapon *weapon, const Unit *attacker) const {
        // Gromril Shields
        if ((m_weaponOption == Ancestral_Weapon_And_Shield) && !weapon->isMissile()) return Rerolls::Failed;

        return CitizenOfSigmar::toSaveRerolls(weapon, attacker);
    }

    std::string Longbeards::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == Ancestral_Weapon_And_Shield) {
                return "Ancestral Weapon and Gromril Shield";
            } else if (parameter.intValue == Ancestral_Great_Axe) {
                return "Ancestral Great Axe";
            }
        }

        return CitizenOfSigmar::ValueToString(parameter);
    }

    int Longbeards::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Ancestral Weapon and Gromril Shield") {
            return Ancestral_Weapon_And_Shield;
        } else if (enumString == "Ancestral Great Axe") {
            return Ancestral_Great_Axe;
        }
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    int Longbeards::runModifier() const {
        auto mod = CitizenOfSigmar::runModifier();
        if (isNamedModelAlive(Model::Musician)) mod++;
        return mod;
    }

    int Longbeards::chargeModifier() const {
        auto mod = CitizenOfSigmar::chargeModifier();
        if (isNamedModelAlive(Model::Musician)) mod++;
        return mod;
    }

    int Longbeards::braveryModifier() const {
        auto mod = CitizenOfSigmar::braveryModifier();
        if (isNamedModelAlive(Model::StandardBearer)) mod++;
        return mod;
    }

    int Longbeards::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace CitiesOfSigmar
