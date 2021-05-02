/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/SkinkPriest.h>
#include <UnitFactory.h>
#include <Board.h>
#include "SeraphonPrivate.h"

namespace Seraphon {
    static const int g_basesize = 25;
    static const int g_wounds = 4;
    static const int g_pointsPerUnit = 70;

    bool SkinkPriest::s_registered = false;

    SkinkPriest::SkinkPriest(WayOfTheSeraphon way, Constellation constellation, CommandTrait trait, Artefact artefact, bool isGeneral) :
            SeraphonBase("Skink Priest", 8, g_wounds, 6, 5, false, g_pointsPerUnit) {
        m_keywords = {ORDER, SERAPHON, SKINK, HERO, PRIEST};
        m_weapons = {&m_starbolt, &m_staff};
        m_battleFieldRole = Role::Leader;

        setWayOfTheSeraphon(way, constellation);
        setArtefact(artefact);
        setCommandTrait(trait);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_starbolt);
        model->addMeleeWeapon(&m_staff);
        addModel(model);
    }

    Unit *SkinkPriest::Create(const ParameterList &parameters) {
        auto way = (WayOfTheSeraphon) GetEnumParam("Way of the Seraphon", parameters, g_wayOfTheSeraphon[0]);
        auto constellation = (Constellation) GetEnumParam("Constellation", parameters, g_constellation[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_skinkCommandTrait[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_vestmentsOfThePriesthood[0]);
        auto general = GetBoolParam("General", parameters, false);

        return new SkinkPriest(way, constellation, trait, artefact, general);
    }

    void SkinkPriest::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SeraphonBase::ValueToString,
                    SeraphonBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Way of the Seraphon", g_wayOfTheSeraphon[0], g_wayOfTheSeraphon),
                            EnumParameter("Constellation", g_constellation[0], g_constellation),
                            EnumParameter("Command Trait", g_skinkCommandTrait[0], g_skinkCommandTrait),
                            EnumParameter("Artefact", g_vestmentsOfThePriesthood[0], g_vestmentsOfThePriesthood),
                            BoolParameter("General")
                    },
                    ORDER,
                    {SERAPHON}
            };
            s_registered = UnitFactory::Register("Skink Priest", factoryMethod);
        }
    }

    int SkinkPriest::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void SkinkPriest::onStartHero(PlayerId player) {
        SeraphonBase::onStartHero(player);

        // Star-stone Staff
        if (owningPlayer() == player) {
            auto skinks = Board::Instance()->getUnitsWithKeyword(owningPlayer(), SKINK);
            for (auto skink : skinks) {
                if ((skink->remainingModels() > 0) && (distanceTo(skink) < 12.0)) {
                    if (Dice::RollD6() >= 3) {
                        const Duration duration{GamePhase::Hero, m_battleRound + 1, owningPlayer()};
                        skink->buffMovement(MovementRule::Run_And_Shoot, true, duration);
                        skink->buffMovement(MovementRule::Run_And_Charge, true, duration);
                        skink->buffModifier(Attribute::To_Save_Missile, 1, duration);
                        skink->buffModifier(Attribute::To_Save_Melee, 1, duration);
                    }
                    break;
                }
            }
        }
    }

} //namespace Seraphon
