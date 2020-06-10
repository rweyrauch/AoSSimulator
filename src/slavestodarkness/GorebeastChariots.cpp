/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <Board.h>
#include "slavestodarkness/GorebeastChariots.h"

namespace SlavesToDarkness {
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 9;
    static const int MIN_UNIT_SIZE = 1;
    static const int MAX_UNIT_SIZE = 3;
    static const int POINTS_PER_BLOCK = 150;
    static const int POINTS_MAX_UNIT_SIZE = 450;

    bool GorebeastChariots::s_registered = false;

    Unit *GorebeastChariots::Create(const ParameterList &parameters) {
        auto unit = new GorebeastChariots();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        auto weapons = (WeaponOption) GetEnumParam("Weapons", parameters, GreatBladeAndWhip);

        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, Ravagers);
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

    void GorebeastChariots::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {GreatBladeAndWhip, WarFlailAndWhip};
            static FactoryMethod factoryMethod = {
                    GorebeastChariots::Create,
                    GorebeastChariots::ValueToString,
                    GorebeastChariots::EnumStringToInt,
                    GorebeastChariots::ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            EnumParameter("Weapons", GreatBladeAndWhip, weapons),
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                            EnumParameter("Mark of Chaos", g_markOfChaos[0], g_markOfChaos),
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS, KHORNE, TZEENTCH, SLAANESH, NURGLE}
            };
            s_registered = UnitFactory::Register("Gorebeast Chariots", factoryMethod);
        }
    }

    GorebeastChariots::GorebeastChariots() :
            SlavesToDarknessBase("Gorebeast Chariots", 9, WOUNDS, 6, 4, false),
            m_greatBlade(Weapon::Type::Melee, "Chaos Greatblade", 2, 2, 3, 3, -1, 2),
            m_flail(Weapon::Type::Melee, "Chaos War-flail", 2, RAND_D6, 4, 3, 0, 1),
            m_whip(Weapon::Type::Melee, "Lashing Whip", 2, 2, 4, 4, 0, 1),
            m_greatBladeExalted(Weapon::Type::Melee, "Chaos Greatblade", 2, 2, 2, 3, -1, 2),
            m_flailExalted(Weapon::Type::Melee, "Chaos War-flail", 2, RAND_D6, 3, 3, 0, 1),
            m_whipExalted(Weapon::Type::Melee, "Lashing Whip", 2, 2, 3, 4, 0, 1),
            m_fists(Weapon::Type::Melee, "Crushing Fists", 1, 3, 3, 3, 0, 2) {
        m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, MARK_OF_CHAOS, GOREBEAST_CHARIOTS};
        m_weapons = {&m_greatBlade, &m_flail, &m_whip, &m_greatBladeExalted, &m_flailExalted, &m_whipExalted, &m_fists};
    }

    bool GorebeastChariots::configure(int numModels, WeaponOption weapons) {
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
        exalted->addMeleeWeapon(&m_fists);
        exalted->setName("Exalted Charioteer");
        addModel(exalted);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            if (weapons == GreatBladeAndWhip)
                model->addMeleeWeapon(&m_greatBlade);
            else if (weapons == WarFlailAndWhip)
                model->addMeleeWeapon(&m_flail);
            model->addMeleeWeapon(&m_whip);
            model->addMeleeWeapon(&m_fists);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    std::string GorebeastChariots::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == GreatBladeAndWhip) {
                return "Chaos Greatblade and Whip";
            } else if (parameter.intValue == WarFlailAndWhip) {
                return "Chaos War-flail and Whip";
            }
        }
        return SlavesToDarknessBase::ValueToString(parameter);
    }

    int GorebeastChariots::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Chaos Greatblade and Whip") {
            return GreatBladeAndWhip;
        } else if (enumString == "Chaos War-flail and Whip") {
            return WarFlailAndWhip;
        }
        return SlavesToDarknessBase::EnumStringToInt(enumString);
    }

    int GorebeastChariots::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

    int GorebeastChariots::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Unit::toHitModifier(weapon, target);

        // Explosive Brutality
        if (m_charged && (m_unmodifiedChargeRoll >= 8)) mod++;

        return mod;
    }

    int GorebeastChariots::toWoundModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Unit::toWoundModifier(weapon, target);

        // Explosive Brutality
        if (m_charged && (m_unmodifiedChargeRoll >= 8)) mod++;

        return mod;
    }

    void GorebeastChariots::onCharged() {
        Unit::onCharged();

        // Crashing Charge
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0);
        for (auto unit : units) {
            if (Dice::rollD6() >= 2) {
                unit->applyDamage({0, Dice::rollD3()});
            }
        }
    }

} //SlavesToDarkness