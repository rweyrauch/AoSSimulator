/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/SaurusVeteranColdOne.h>
#include <UnitFactory.h>
#include "SeraphonPrivate.h"

namespace Seraphon {
    static const int g_basesize = 60; // x35 oval
    static const int g_wounds = 7;
    static const int g_pointsPerUnit = 110;

    bool SaurusScarVeteranOnColdOne::s_registered = false;

    SaurusScarVeteranOnColdOne::SaurusScarVeteranOnColdOne(WayOfTheSeraphon way, Constellation constellation, CommandTrait trait, Artefact artefact, bool isGeneral) :
            SeraphonBase("Saurus Scar-Veteran on Cold One", 8, g_wounds, 8, 4, false, g_pointsPerUnit) {
        m_keywords = {ORDER, SERAPHON, SAURUS, HERO, COLD_ONE, SCAR_VETERAN};
        m_weapons = {&m_warpick, &m_jaws, &m_coldOneJaws};
        m_battleFieldRole = Role::Leader;
        m_hasMount = true;
        m_coldOneJaws.setMount(true);

        setWayOfTheSeraphon(way, constellation);
        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_warpick);
        model->addMeleeWeapon(&m_jaws);
        model->addMeleeWeapon(&m_coldOneJaws);
        addModel(model);

        m_commandAbilities.push_back(std::make_unique<BuffAbilityCommandAbility>(this, "Saurian Savagery", 18, 18, GamePhase::Combat,
                                                                                 Ability::Extra_Hit_On_Value, 6, Abilities::Target::SelfAndFriendly,
                                                                                 std::vector<Keyword>(SAURUS)));
    }

    Unit *SaurusScarVeteranOnColdOne::Create(const ParameterList &parameters) {
        auto way = (WayOfTheSeraphon) GetEnumParam("Way of the Seraphon", parameters, g_wayOfTheSeraphon[0]);
        auto constellation = (Constellation) GetEnumParam("Constellation", parameters, g_constellation[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_saurusCommandTrait[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_celestialRelicsOfTheWarrior[0]);
        auto general = GetBoolParam("General", parameters, false);

        return new SaurusScarVeteranOnColdOne(way, constellation, trait, artefact, general);
    }

    void SaurusScarVeteranOnColdOne::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SeraphonBase::ValueToString,
                    SeraphonBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Way of the Seraphon", g_wayOfTheSeraphon[0], g_wayOfTheSeraphon),
                            EnumParameter("Constellation", g_constellation[0], g_constellation),
                            EnumParameter("Command Trait", g_saurusCommandTrait[0], g_saurusCommandTrait),
                            EnumParameter("Artefact", g_celestialRelicsOfTheWarrior[0], g_celestialRelicsOfTheWarrior),
                            BoolParameter("General")
                    },
                    ORDER,
                    {SERAPHON}
            };

            s_registered = UnitFactory::Register("Saurus Scar-Veteran on Cold One", factoryMethod);
        }
    }

    int SaurusScarVeteranOnColdOne::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        // Cold Ferocity
        if ((unmodifiedHitRoll == 6) && (weapon->name() == m_warpick.name())) {
            return 2;
        }
        return SeraphonBase::generateHits(unmodifiedHitRoll, weapon, unit);
    }

    int SaurusScarVeteranOnColdOne::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} //namespace Seraphon
