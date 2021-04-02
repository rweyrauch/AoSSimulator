/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/CelestantPrime.h>
#include <Board.h>
#include "UnitFactory.h"
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int g_basesize = 100; //
    static const int g_wounds = 8;
    static const int g_pointsPerUnit = 300;

    bool CelestantPrime::s_registered = false;

    Unit *CelestantPrime::Create(const ParameterList &parameters) {
        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, g_stormhost[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTrait[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefactsOfTheTempests[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new CelestantPrime(stormhost, trait, artefact, general);
    }

    void CelestantPrime::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Stormhost", g_stormhost[0], g_stormhost),
                            EnumParameter("Command Trait", g_commandTrait[0], g_commandTrait),
                            BoolParameter("General")
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Celestant Prime", factoryMethod);
        }
    }

    std::string CelestantPrime::ValueToString(const Parameter &parameter) {
        return StormcastEternal::ValueToString(parameter);
    }

    int CelestantPrime::EnumStringToInt(const std::string &enumString) {
        return StormcastEternal::EnumStringToInt(enumString);
    }

    CelestantPrime::CelestantPrime(Stormhost stormhost, CommandTrait trait, Artefact artefact, bool isGeneral) :
            StormcastEternal(stormhost,"Celestant Prime", 12, g_wounds, 10, 3, true),
            m_ghalMaraz(Weapon::Type::Melee, "Ghal Maraz, the Hammer of Sigmar", 2, 3, 3, 2, -3, 3) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, HERO, CELESTANT_PRIME};
        m_weapons = {&m_ghalMaraz};
        m_battleFieldRole = Role::Leader;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        s_globalBraveryMod.connect(this, &CelestantPrime::bearerOfTheWarhammer, &m_connection);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_ghalMaraz);
        addModel(model);

        m_points = g_pointsPerUnit;
    }

    CelestantPrime::~CelestantPrime() {
        m_connection.disconnect();
    }

    int CelestantPrime::bearerOfTheWarhammer(const Unit *target) {
        // Bearer of the Warhammer
        if (target->hasKeyword(ORDER) && (target->owningPlayer() == owningPlayer()) && (distanceTo(target) <= 18.0)) {
            return 1;
        }

        return 0;
    }

    int CelestantPrime::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    void CelestantPrime::onStartShooting(PlayerId player) {
        StormcastEternal::onStartShooting(player);

        // Cometstrike Sceptre
        if (owningPlayer() == player) {
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 24.0);
            for (auto unit : units) {
                if (unit->remainingModels() > 0) {
                    unit->applyDamage({0, Dice::RollD3(), Wounds::Source::Ability}, this);
                    auto targetUnits = Board::Instance()->getUnitsWithin(unit, unit->owningPlayer(), Dice::RollD6());
                    for (auto target : targetUnits) {
                        target->applyDamage({0, Dice::RollD3(), Wounds::Source::Ability}, this);
                    }
                    break;
                }
            }
        }
    }


} // namespace StormcastEternals