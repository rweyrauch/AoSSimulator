/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/KnightQuestor.h>
#include <iostream>
#include "UnitFactory.h"
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int g_basesize = 40;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 100;

    bool KnightQuestor::s_registered = false;

    KnightQuestor::KnightQuestor(Stormhost stormhost, CommandTrait trait, Artefact artefact, bool isGenera) :
            StormcastEternal(stormhost, "Knight-Questor", 5, g_wounds, 8, 3, false),
            m_warblade(Weapon::Type::Melee, "Questor Warblade", 1, 4, 3, 3, -1, 1) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, HERO, KNIGHT_QUESTOR};
        m_weapons = {&m_warblade};
        m_battleFieldRole = Role::Leader;

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_warblade);
        addModel(model);

        m_points = g_pointsPerUnit;
    }

    Unit *KnightQuestor::Create(const ParameterList &parameters) {
        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, g_stormhost[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTrait[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new KnightQuestor(stormhost, trait, artefact, general);
    }

    void KnightQuestor::Init() {
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

            s_registered = UnitFactory::Register("Knight-Questor", factoryMethod);
        }
    }

    Rerolls KnightQuestor::toHitRerolls(const Weapon *weapon, const Unit *unit) const {
        // Heroic Challenge
        if (unit->hasKeyword(HERO)) {
            return Rerolls::Failed;
        }
        return StormcastEternal::toHitRerolls(weapon, unit);
    }

    Rerolls KnightQuestor::toSaveRerolls(const Weapon * /*weapon*/, const Unit *attacker) const {
        // Sigmarite Shield
        return Rerolls::Failed;
    }

    Wounds KnightQuestor::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Thundercharged Strike
        if (woundRoll == 6 && weapon->name() == m_warblade.name()) {
            return {2, 0};
        }
        return StormcastEternal::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int KnightQuestor::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }


} // namespace StormcastEternals