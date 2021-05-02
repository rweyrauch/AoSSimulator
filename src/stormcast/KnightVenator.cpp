/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/KnightVenator.h>
#include <iostream>
#include <Board.h>
#include "UnitFactory.h"
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int g_basesize = 40;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 110;

    bool KnightVenator::s_registered = false;

    KnightVenator::KnightVenator(Stormhost stormhost, CommandTrait trait, Artefact artefact, bool isGeneral) :
            StormcastEternal(stormhost, "Knight-Venator", 12, g_wounds, 9, 3, true, g_pointsPerUnit),
            m_realmhuntersBow(Weapon::Type::Missile, "Realmhunter's Bow", 30, 3, 2, 3, -1, 1),
            m_starFatedArrow(Weapon::Type::Missile, "Realmhunter's Bow - Star-fated Arrow", 30, 1, 2, 3, -1, 0),
            m_beakAndTalonsMissile(Weapon::Type::Missile, "Celestial Beak and Talons", 30, 3, 4, 3, 0, 1),
            m_bowStave(Weapon::Type::Melee, "Bow Stave", 1, 2, 3, 4, 0, 1),
            m_beakAndTalons(Weapon::Type::Melee, "Celestial Beak and Talons", 1, 3, 4, 3, 0, 1) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, HERO, KNIGHT_VENATOR};
        m_weapons = {&m_realmhuntersBow, &m_beakAndTalonsMissile, &m_bowStave, &m_beakAndTalons};
        m_battleFieldRole = Role::Leader;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_realmhuntersBow);
        model->addMissileWeapon(&m_starFatedArrow);
        m_starFatedArrow.activate(false);
        model->addMissileWeapon(&m_beakAndTalonsMissile);
        model->addMeleeWeapon(&m_bowStave);
        model->addMeleeWeapon(&m_beakAndTalons);
        addModel(model);

        m_points = g_pointsPerUnit;
    }

    Unit *KnightVenator::Create(const ParameterList &parameters) {
        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, g_stormhost[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTrait[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new KnightVenator(stormhost, trait, artefact, general);
    }

    void KnightVenator::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    StormcastEternal::ValueToString,
                    StormcastEternal::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Stormhost", g_stormhost[0], g_stormhost),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            EnumParameter("Command Trait", g_commandTrait[0], g_commandTrait),
                            BoolParameter("General")
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Knight-Venator", factoryMethod);
        }
    }

    int KnightVenator::weaponRend(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Celestial Strike
        if ((woundRoll == 6) && (weapon->name() == m_beakAndTalonsMissile.name())) {
            return -3;
        }
        return StormcastEternal::weaponRend(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int KnightVenator::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void KnightVenator::onStartShooting(PlayerId player) {
        StormcastEternal::onStartShooting(player);

        m_realmhuntersBow.activate(true);

        if ((owningPlayer() == player) && !m_usedStarFatedArrow) {
            auto targets = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()),
                                                             m_starFatedArrow.range());
            for (auto unit : targets) {
                if ((unit->remainingModels() > 0) && (unit->hasKeyword(MONSTER) || unit->hasKeyword(HERO))) {
                    m_starFatedArrow.activate(true);
                    m_realmhuntersBow.activate(false);
                    m_meleeTarget = unit;
                    m_usedStarFatedArrow = true;
                }
            }
        }
    }

    Wounds KnightVenator::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        if (weapon->name() == m_starFatedArrow.name()) {
            if ((target->hasKeyword(HERO) || target->hasKeyword(MONSTER))) {
                return {Dice::RollD6() + 3, 0, Wounds::Source::Weapon_Missile};
            } else {
                return {Dice::RollD3() + 3, 0, Wounds::Source::Weapon_Missile};
            }
        }
        return StormcastEternal::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }


} // namespace StormcastEternals