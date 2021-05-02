/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/RipperdactylChief.h>
#include <UnitFactory.h>
#include "SeraphonPrivate.h"

namespace Seraphon {
    static const int g_basesize = 50;
    static const int g_wounds = 4;
    static const int g_pointsPerUnit = 80;

    bool RipperdactylChief::s_registered = false;

    RipperdactylChief::RipperdactylChief(WayOfTheSeraphon way, Constellation constellation, CommandTrait trait, Artefact artefact, bool isGeneral) :
            SeraphonBase("Ripperdactyl Chief", 12, g_wounds, 6, 5, true, g_pointsPerUnit) {
        m_keywords = {ORDER, SERAPHON, SKINK, RIPPERDACTYL, HERO, RIPPERDACTYL_CHIEF};
        m_weapons = {&m_skyblade, &m_jaws};
        m_battleFieldRole = Role::Leader;
        m_hasMount = true;
        m_jaws.setMount(true);

        setWayOfTheSeraphon(way, constellation);
        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto alpha = new Model(g_basesize, wounds());
        alpha->addMeleeWeapon(&m_skyblade);
        alpha->addMeleeWeapon(&m_jaws);
        addModel(alpha);
    }

    Unit *RipperdactylChief::Create(const ParameterList &parameters) {
        auto way = (WayOfTheSeraphon) GetEnumParam("Way of the Seraphon", parameters, g_wayOfTheSeraphon[0]);
        auto constellation = (Constellation) GetEnumParam("Constellation", parameters, g_constellation[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_skinkCommandTrait[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_vestmentsOfThePriesthood[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new RipperdactylChief(way, constellation, trait, artefact, general);
    }

    void RipperdactylChief::Init() {
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

            s_registered = UnitFactory::Register("Ripperdactyl Chief", factoryMethod);
        }
    }

    int RipperdactylChief::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        // Voracious Appetite
        if ((unmodifiedHitRoll == 6) && (weapon->name() == m_jaws.name())) {
            return 2;
        }
        return SeraphonBase::generateHits(unmodifiedHitRoll, weapon, unit);
    }

    int RipperdactylChief::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} //namespace Seraphon
