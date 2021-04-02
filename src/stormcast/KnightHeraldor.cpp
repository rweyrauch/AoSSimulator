/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/KnightHeraldor.h>
#include <iostream>
#include <Board.h>
#include "UnitFactory.h"
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int g_basesize = 40;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 100;

    bool KnightHeraldor::s_registered = false;

    KnightHeraldor::KnightHeraldor(Stormhost stormhost, CommandTrait trait, Artefact artefact, bool isGeneral) :
            StormcastEternal(stormhost, "Knight-Heraldor", 5, g_wounds, 8, 3, false),
            m_broadsword(Weapon::Type::Melee, "Sigmarite Broadsword", 1, 4, 3, 4, -1, 1) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, HERO, KNIGHT_HERALDOR};
        m_weapons = {&m_broadsword};
        m_battleFieldRole = Role::Leader;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_broadsword);
        addModel(model);

        m_points = g_pointsPerUnit;
    }

    Unit *KnightHeraldor::Create(const ParameterList &parameters) {
        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, g_stormhost[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTrait[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefactsOfTheTempests[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new KnightHeraldor(stormhost, trait, artefact, general);
    }

    void KnightHeraldor::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    StormcastEternal::ValueToString,
                    StormcastEternal::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Stormhost", g_stormhost[0], g_stormhost),
                            EnumParameter("Command Trait", g_commandTrait[0], g_commandTrait),
                            BoolParameter("General")
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Knight-Heraldor", factoryMethod);
        }
    }

    int KnightHeraldor::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    void KnightHeraldor::onStartHero(PlayerId player) {
        StormcastEternal::onStartHero(player);

        if (owningPlayer() == player) {
            auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 12.0);
            for (auto unit : units) {
                if ((unit->remainingModels() > 0) && unit->hasKeyword(STORMCAST_ETERNAL)) {
                    unit->buffMovement(MovementRule::Run_And_Charge, true,
                                       {Phase::Combat, m_battleRound, owningPlayer()});
                    unit->buffMovement(MovementRule::Retreat_And_Charge, true,
                                       {Phase::Combat, m_battleRound, owningPlayer()});
                }
            }
        }
    }


} // namespace StormcastEternals