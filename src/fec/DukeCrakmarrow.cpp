/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <fec/DukeCrakmarrow.h>
#include <UnitFactory.h>
#include <Board.h>
#include <iostream>
#include "FleshEaterCourtsPrivate.h"

namespace FleshEaterCourt {
    static const int g_basesize = 40;
    static const int g_wounds = 4;
    static const int g_pointsPerUnit = 120;

    bool DukeCrakmarrow::s_registered = false;

    DukeCrakmarrow::DukeCrakmarrow() :
            FleshEaterCourts("Duke Crakmarrow", 6, g_wounds, 10, 5, false),
            m_halberd(Weapon::Type::Melee, "Halberd", 2, 3, 3, 3, -1, 2) {
        m_keywords = {DEATH, MORDANT, FLESH_EATER_COURTS, HOLLOWMOURNE, HERO, COURTIER, DUKE_CRAKMARROW};
        m_weapons = {&m_halberd};
        m_battleFieldRole = Role::Leader;
    }

    bool DukeCrakmarrow::configure() {
        auto infernal = new Model(g_basesize, wounds());
        infernal->addMeleeWeapon(&m_halberd);
        addModel(infernal);

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *DukeCrakmarrow::Create(const ParameterList &parameters) {
        auto unit = new DukeCrakmarrow();

        unit->setGrandCourt(GrandCourt::Hollowmourne);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure();
        return unit;
    }

    void DukeCrakmarrow::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    DukeCrakmarrow::Create,
                    FleshEaterCourts::ValueToString,
                    FleshEaterCourts::EnumStringToInt,
                    DukeCrakmarrow::ComputePoints,
                    {
                            BoolParameter("General")
                    },
                    DEATH,
                    {FLESH_EATER_COURTS}
            };
            s_registered = UnitFactory::Register("Duke Crakmarrow", factoryMethod);
        }
    }

    int DukeCrakmarrow::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    void DukeCrakmarrow::onStartHero(PlayerId player) {
        FleshEaterCourts::onStartHero(player);

        // Muster the Grymwatch
        auto grymwatch = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), GRYMWATCH, 3.0);
        if (grymwatch) {
            Dice::RollResult rolls;
            Dice::RollD6(6, rolls);
            grymwatch->returnModels(rolls.rollsGE(4));
        }
    }

    Wounds DukeCrakmarrow::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        auto grymwatch = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), GRYMWATCH, 3.0);
        if (grymwatch) {
            if (Dice::RollD6() >= 4) {
                grymwatch->applyDamage(wounds, attackingUnit);
                return {0, 0, wounds.source};
            }
        }
        return FleshEaterCourts::applyWoundSave(wounds, attackingUnit);
    }

    Wounds DukeCrakmarrow::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Gallent Champion
        if (target->hasKeyword(MONSTER)) {
            return {weapon->damage() + 1, 0, Wounds::Source::Weapon_Melee};
        }
        return FleshEaterCourts::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

} // namespace FleshEaterCourt
