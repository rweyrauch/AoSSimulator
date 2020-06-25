/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/SkinkStarseer.h>
#include <UnitFactory.h>
#include <Board.h>
#include <Roster.h>
#include "SeraphonPrivate.h"

namespace Seraphon {
    static const int BASESIZE = 50;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 140;

    bool SkinkStarseer::s_registered = false;

    SkinkStarseer::SkinkStarseer() :
            SeraphonBase("Skink Starseer", 5, WOUNDS, 6, 5, true),
            m_astralBolt(Weapon::Type::Missile, "Astral Bolt", 18, 2, 3, 3, -1, RAND_D3),
            m_staff(Weapon::Type::Melee, "Astromancer's Staff", 2, 2, 4, 3, -1, RAND_D3) {
        m_keywords = {ORDER, SERAPHON, SKINK, HERO, WIZARD, STARSEER};
        m_weapons = {&m_astralBolt, &m_staff};
    }

    bool SkinkStarseer::configure(Lore lore) {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_astralBolt);
        model->addMeleeWeapon(&m_staff);
        addModel(model);

        m_points = ComputePoints(1);

        return true;
    }

    Unit *SkinkStarseer::Create(const ParameterList &parameters) {
        auto unit = new SkinkStarseer();

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

    void SkinkStarseer::Init() {
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

            s_registered = UnitFactory::Register("Skink Starseer", factoryMethod);
        }
    }

    void SkinkStarseer::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        // Cosmic Herald
        if (owningPlayer() == player) {
            Dice::RollResult result;
            Dice::rollD6(1, result);
            m_roster->addCommandPoints(result.rollsGE(4));
        }
    }

    int SkinkStarseer::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} //namespace Seraphon
