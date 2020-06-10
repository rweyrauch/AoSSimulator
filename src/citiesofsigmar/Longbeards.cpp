/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <citiesofsigmar/Longbeards.h>
#include <UnitFactory.h>

namespace CitiesOfSigmar {
    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 110;
    static const int POINTS_MAX_UNIT_SIZE = 270;

    bool Longbeards::s_registered = false;

    Longbeards::Longbeards() :
            CitizenOfSigmar("Longbeards", 4, WOUNDS, 7, 4, false),
            m_ancestralAxeHammer(Weapon::Type::Melee, "Ancestral Axe or Ancestral Hammer", 1, 1, 3, 4, 0, 1),
            m_ancestralGreatAxe(Weapon::Type::Melee, "Ancestral Great Axe", 1, 1, 4, 3, -1, 1),
            m_ancestralAxeHammerOldGuard(Weapon::Type::Melee, "Ancestral Axe or Ancestral Hammer", 1, 2, 3, 4, 0, 1),
            m_ancestralGreatAxeOldGuard(Weapon::Type::Melee, "Ancestral Great Axe", 1, 2, 4, 3, -1, 1) {
        m_keywords = {ORDER, DUARDIN, DISPOSSESSED, LONGBEARDS};
        m_weapons = {&m_ancestralAxeHammer, &m_ancestralGreatAxe, &m_ancestralAxeHammerOldGuard,
                     &m_ancestralGreatAxeOldGuard};
    }

    bool Longbeards::configure(int numModels, WeaponOptions weapons, bool standardBearer, bool musician) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        m_standardBearer = standardBearer;
        m_musician = musician;

        auto oldguard = new Model(BASESIZE, wounds());
        if (weapons == AncestralWeaponAndShield) {
            oldguard->addMeleeWeapon(&m_ancestralAxeHammerOldGuard);
        } else if (weapons == AncestralGreatAxe) {
            oldguard->addMeleeWeapon(&m_ancestralGreatAxeOldGuard);
        }
        addModel(oldguard);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            if (weapons == AncestralWeaponAndShield) {
                model->addMeleeWeapon(&m_ancestralAxeHammer);
            } else if (weapons == AncestralGreatAxe) {
                model->addMeleeWeapon(&m_ancestralGreatAxe);
            }
            addModel(model);
        }

        m_weaponOption = weapons;
        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *Longbeards::Create(const ParameterList &parameters) {
        auto unit = new Longbeards();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        auto weapons = (WeaponOptions) GetEnumParam("Weapons", parameters, AncestralWeaponAndShield);
        bool standardBearer = GetBoolParam("Standard Bearer", parameters, false);
        bool musician = GetBoolParam("Musician", parameters, false);

        auto city = (City) GetEnumParam("City", parameters, Hammerhal);
        unit->setCity(city);

        bool ok = unit->configure(numModels, weapons, standardBearer, musician);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Longbeards::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {AncestralWeaponAndShield, AncestralGreatAxe};
            static FactoryMethod factoryMethod = {
                    Longbeards::Create,
                    Longbeards::ValueToString,
                    Longbeards::EnumStringToInt,
                    Longbeards::ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            EnumParameter("Weapons", AncestralWeaponAndShield, weapons),
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

    Rerolls Longbeards::toSaveRerolls(const Weapon *weapon) const {
        // Gromril Shields
        if ((m_weaponOption == AncestralWeaponAndShield) && !weapon->isMissile()) return RerollFailed;

        return CitizenOfSigmar::toSaveRerolls(weapon);
    }

    std::string Longbeards::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == AncestralWeaponAndShield) {
                return "Ancestral Weapon and Gromril Shield";
            } else if (parameter.intValue == AncestralGreatAxe) {
                return "Ancestral Great Axe";
            }
        }

        return CitizenOfSigmar::ValueToString(parameter);
    }

    int Longbeards::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Ancestral Weapon and Gromril Shield") {
            return AncestralWeaponAndShield;
        } else if (enumString == "Ancestral Great Axe") {
            return AncestralGreatAxe;
        }
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    int Longbeards::runModifier() const {
        auto mod = Unit::runModifier();
        if (m_musician) mod++;
        return mod;
    }

    int Longbeards::chargeModifier() const {
        auto mod = Unit::chargeModifier();
        if (m_musician) mod++;
        return mod;
    }

    int Longbeards::braveryModifier() const {
        auto mod = Unit::braveryModifier();
        if (m_standardBearer) mod++;
        return mod;
    }

    int Longbeards::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace CitiesOfSigmar
