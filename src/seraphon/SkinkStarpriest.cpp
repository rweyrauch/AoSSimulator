/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/SkinkStarpriest.h>
#include <UnitFactory.h>
#include <Board.h>
#include <Roster.h>
#include "SeraphonPrivate.h"

namespace Seraphon {
    static const int BASESIZE = 25;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 120;

    bool SkinkStarpriest::s_registered = false;

    SkinkStarpriest::SkinkStarpriest() :
            SeraphonBase("Skink Starpriest", 8, WOUNDS, 6, 5, false),
            m_venombolt(Weapon::Type::Missile, "Venombolt", 18, 2, 3, 3, -1, 1),
            m_staff(Weapon::Type::Melee, "Serpent Staff", 1, 2, 4, 3, -1, 1) {
        m_keywords = {ORDER, SERAPHON, SKINK, HERO, WIZARD, STARPRIEST};
        m_weapons = {&m_venombolt, &m_staff};
        m_battleFieldRole = Leader;
    }

    bool SkinkStarpriest::configure(Lore lore) {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_venombolt);
        model->addMeleeWeapon(&m_staff);
        addModel(model);

        m_points = ComputePoints(1);

        return true;
    }

    Unit *SkinkStarpriest::Create(const ParameterList &parameters) {
        auto unit = new SkinkStarpriest();

        auto way = (WayOfTheSeraphon) GetEnumParam("Way of the Seraphon", parameters, g_wayOfTheSeraphon[0]);
        auto constellation = (Constellation) GetEnumParam("Constellation", parameters, g_constellation[0]);
        unit->setWayOfTheSeraphon(way, constellation);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_loreOfCelestialManipulation[0]);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_skinkCommandTrait[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_vestmentsOfThePriesthood[0]);

        unit->setArtefact(artefact);
        unit->setCommandTrait(trait);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void SkinkStarpriest::Init() {
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
                            EnumParameter("Lore", g_loreOfCelestialManipulation[0], g_loreOfCelestialManipulation),
                            BoolParameter("General")
                    },
                    ORDER,
                    {SERAPHON}
            };

            s_registered = UnitFactory::Register("Skink Starpriest", factoryMethod);
        }
    }

    void SkinkStarpriest::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        // Astral Herald
        if (owningPlayer() == player) {
            Dice::RollResult result;
            Dice::rollD6(1, result);
            m_roster->addCommandPoints(result.rollsGE(5));
        }
    }

    int SkinkStarpriest::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} //namespace Seraphon
