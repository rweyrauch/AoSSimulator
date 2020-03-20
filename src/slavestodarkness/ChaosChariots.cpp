/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <Board.h>
#include "slavestodarkness/ChaosChariots.h"

namespace SlavesToDarkness {
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 7;
    static const int MIN_UNIT_SIZE = 1;
    static const int MAX_UNIT_SIZE = 3;
    static const int POINTS_PER_BLOCK = 120;
    static const int POINTS_MAX_UNIT_SIZE = 360;

    bool ChaosChariots::s_registered = false;

    Unit *ChaosChariots::Create(const ParameterList &parameters) {
        auto unit = new ChaosChariots();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        auto weapons = (WeaponOption) GetEnumParam("Weapons", parameters, GreatBladeAndWhip);

        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, SlavesToDarknessBase::Ravagers);
        unit->setDamnedLegion(legion);

        auto mark = (MarkOfChaos) GetEnumParam("Mark of Chaos", parameters, Undivided);
        unit->setMarkOfChaos(mark);

        bool ok = unit->configure(numModels, weapons);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void ChaosChariots::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    ChaosChariots::Create,
                    ChaosChariots::ValueToString,
                    ChaosChariots::EnumStringToInt,
                    ChaosChariots::ComputePoints,
                    {
                            {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                            {
                                    ParamType::Enum, "Weapons", ChaosChariots::GreatBladeAndWhip,
                                    ChaosChariots::GreatBladeAndWhip,
                                    ChaosChariots::WarFlailAndWhip, 1
                            },
                            {ParamType::Enum, "Damned Legion", SlavesToDarknessBase::Ravagers,
                             SlavesToDarknessBase::Ravagers, SlavesToDarknessBase::HostOfTheEverchosen, 1},
                            {ParamType::Enum, "Mark of Chaos", SlavesToDarknessBase::Undivided,
                             SlavesToDarknessBase::Undivided, SlavesToDarknessBase::Tzeentch},
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS, KHORNE, TZEENTCH, SLAANESH, NURGLE}
            };
            s_registered = UnitFactory::Register("Chaos Chariots", factoryMethod);
        }
    }

    ChaosChariots::ChaosChariots() :
            SlavesToDarknessBase("Chaos Chariots", 12, WOUNDS, 6, 4, false),
            m_greatBlade(Weapon::Type::Melee, "Chaos Greatblade", 2, 2, 3, 3, -1, 2),
            m_flail(Weapon::Type::Melee, "Chaos War-flail", 2, RAND_D6, 4, 3, 0, 1),
            m_whip(Weapon::Type::Melee, "Lashing Whip", 2, 2, 4, 4, 0, 1),
            m_greatBladeExalted(Weapon::Type::Melee, "Chaos Greatblade", 2, 2, 2, 3, -1, 2),
            m_flailExalted(Weapon::Type::Melee, "Chaos War-flail", 2, RAND_D6, 3, 3, 0, 1),
            m_whipExalted(Weapon::Type::Melee, "Lashing Whip", 2, 2, 3, 4, 0, 1),
            m_hooves(Weapon::Type::Melee, "Trampling Hooves", 1, 4, 4, 4, 0, 1) {
        m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, MARK_OF_CHAOS, CHAOS_CHARIOTS};
        m_weapons = {&m_greatBlade, &m_flail, &m_whip, &m_greatBladeExalted, &m_flailExalted, &m_whipExalted,
                     &m_hooves};
    }

    bool ChaosChariots::configure(int numModels, WeaponOption weapons) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        auto exalted = new Model(BASESIZE, wounds());
        if (weapons == GreatBladeAndWhip) {
            exalted->addMeleeWeapon(&m_greatBladeExalted);
        } else if (weapons == WarFlailAndWhip) {
            exalted->addMeleeWeapon(&m_flailExalted);
        }
        exalted->addMeleeWeapon(&m_whipExalted);
        exalted->addMeleeWeapon(&m_hooves);
        exalted->setName("Exalted Charioteer");
        addModel(exalted);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            if (weapons == GreatBladeAndWhip)
                model->addMeleeWeapon(&m_greatBlade);
            else if (weapons == WarFlailAndWhip)
                model->addMeleeWeapon(&m_flail);
            model->addMeleeWeapon(&m_whip);
            model->addMeleeWeapon(&m_hooves);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    std::string ChaosChariots::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == GreatBladeAndWhip) {
                return "Chaos Greatblade and Whip";
            } else if (parameter.intValue == WarFlailAndWhip) {
                return "Chaos War-flail and Whip";
            }
        }
        return SlavesToDarknessBase::ValueToString(parameter);
    }

    int ChaosChariots::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Chaos Greatblade and Whip") {
            return GreatBladeAndWhip;
        } else if (enumString == "Chaos War-flail and Whip") {
            return WarFlailAndWhip;
        }
        return SlavesToDarknessBase::EnumStringToInt(enumString);
    }

    int ChaosChariots::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

    void ChaosChariots::onRestore() {
        Unit::onRestore();

        m_runAndCharge = true;
    }

    void ChaosChariots::onCharged() {
        Unit::onCharged();

        // Don't Spare the Lash
        if (m_ran && m_charged) {
            // Can only run and charge once per battle
            m_runAndCharge = false;
        }

        // Swift Death
        auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
        if (unit && (distanceTo(unit) <= 1.0f)) {
            Dice::RollResult result;
            Dice::rollD6(m_unmodifiedChargeRoll, result);
            unit->applyDamage({0, result.rollsGE(5)});
        }
    }

} //SlavesToDarkness