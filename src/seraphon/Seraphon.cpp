/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/Seraphon.h>
#include "seraphon/DreadSaurian.h"
#include "seraphon/SaurusGuard.h"
#include "seraphon/SaurusKnights.h"
#include "seraphon/SaurusWarriors.h"
#include "seraphon/Skinks.h"
#include "seraphon/SaurusOldblood.h"
#include "seraphon/SaurusSunblood.h"
#include "seraphon/SaurusOldbloodCarnosaur.h"
#include "seraphon/SaurusVeteranCarnosaur.h"
#include "seraphon/Troglodon.h"
#include "seraphon/Bastiladon.h"
#include "seraphon/Salamanders.h"
#include "seraphon/Razordons.h"
#include "seraphon/Kroxigor.h"
#include "seraphon/RipperdactylRiders.h"
#include "seraphon/TerradonRiders.h"
#include "seraphon/Stegadon.h"
#include "seraphon/EngineOfTheGods.h"
#include "seraphon/SaurusVeteranColdOne.h"
#include "seraphon/ChameleonSkinks.h"
#include "seraphon/RipperdactylChief.h"
#include "seraphon/TerradonChief.h"
#include "seraphon/SaurusEternityWarden.h"
#include "seraphon/SaurusAstrolithBearer.h"
#include "seraphon/LordKroak.h"
#include "seraphon/SkinkStarseer.h"
#include "seraphon/SkinkStarpriest.h"
#include "seraphon/SkinkPriest.h"
#include "seraphon/SlannStarmaster.h"
#include "../AoSSimPrivate.h"

namespace Seraphon {

    std::string SeraphonBase::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Way of the Seraphon") {
            auto wayName = magic_enum::enum_name((WayOfTheSeraphon)parameter.intValue);
            return std::string(wayName);
        } else if (std::string(parameter.name) == "Constellation") {
            auto constName = magic_enum::enum_name((Constellation)parameter.intValue);
            return std::string(constName);
        } else if (std::string(parameter.name) == "Command Trait") {
            auto traitName = magic_enum::enum_name((CommandTrait)parameter.intValue);
            return std::string(traitName);
        } else if (std::string(parameter.name) == "Artefact") {
            auto artefactName = magic_enum::enum_name((Artefact)parameter.intValue);
            return std::string(artefactName);
        } else if ((std::string(parameter.name) == "Lore of Celestial Manipulation") ||
                   (std::string(parameter.name) == "Lore") ||
                   (std::string(parameter.name) == "Lore of Celestial Domination")) {
            auto loreName = magic_enum::enum_name((Lore)parameter.intValue);
            return std::string(loreName);
        }
        return ParameterValueToString(parameter);
    }

    int SeraphonBase::EnumStringToInt(const std::string &enumString) {
        auto way = magic_enum::enum_cast<WayOfTheSeraphon>(enumString);
        if (way.has_value()) return (int)way.value();

        auto constellation = magic_enum::enum_cast<Constellation>(enumString);
        if (constellation.has_value()) return (int)constellation.value();

        auto lore = magic_enum::enum_cast<Lore>(enumString);
        if (lore.has_value()) return (int)lore.value();

        auto artefact = magic_enum::enum_cast<Artefact>(enumString);
        if (artefact.has_value()) return (int)artefact.value();

        auto trait = magic_enum::enum_cast<CommandTrait>(enumString);
        if (trait.has_value()) return (int)trait.value();

        return 0;
    }

    void SeraphonBase::setWayOfTheSeraphon(Seraphon::WayOfTheSeraphon way, Constellation constellation) {
        // TODO: validate the combination of way and constellation and warn caller
        if (way == WayOfTheSeraphon::Starborne) {
            if (constellation == Constellation::Koatls_Claw || constellation == Constellation::Thunder_Lizard)
                constellation = Constellation::None;
        } else if (way == WayOfTheSeraphon::Coalesced) {
            if (constellation == Constellation::Dracothions_Tail || constellation == Constellation::Fangs_Of_Sotek)
                constellation = Constellation::None;
        }

        removeKeyword(COALESCED);
        removeKeyword(STARBORNE);
        removeKeyword(KOATLS_CLAW);
        removeKeyword(THUNDER_LIZARD);
        removeKeyword(DRACOTHIONS_TAIL);
        removeKeyword(FANGS_OF_SOTEK);

        if (way == WayOfTheSeraphon::Coalesced) addKeyword(COALESCED);
        else if (way == WayOfTheSeraphon::Starborne) addKeyword(STARBORNE);

        if (constellation == Constellation::Koatls_Claw) addKeyword(KOATLS_CLAW);
        else if (constellation == Constellation::Thunder_Lizard) addKeyword(THUNDER_LIZARD);
        else if (constellation == Constellation::Dracothions_Tail) addKeyword(DRACOTHIONS_TAIL);
        else if (constellation == Constellation::Fangs_Of_Sotek) addKeyword(FANGS_OF_SOTEK);

        m_way = way;
        m_constellation = constellation;
    }

    int SeraphonBase::moveModifier() const {
        auto mod = Unit::moveModifier();

        // First to Battle
        if ((m_battleRound == 1) && (hasKeyword(SKINK) && (hasKeyword(FANGS_OF_SOTEK)))) {
            mod += 3;
        }

        return mod;
    }

    int SeraphonBase::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Unit::toHitModifier(weapon, target);

        // Savagery Incarnate
        if (m_charged && hasKeyword(KOATLS_CLAW) && hasKeyword(SAURUS)) mod++;

        return mod;
    }

    int SeraphonBase::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto extra = Unit::extraAttacks(attackingModel, weapon, target);

        // Predatory Fighters
        if (hasKeyword(COALESCED) && (weapon->name().find("Jaws") != std::string::npos)) {
            extra++;
        }

        return extra;
    }

    int SeraphonBase::braveryModifier() const {
        auto mod = Unit::braveryModifier();

        // Unfeeling
        if (hasKeyword(STARBORNE)) {
            // All Starborne units have a bravery of 10.
            mod += (10 - bravery());
        }
        return mod;
    }

    Wounds SeraphonBase::targetAttackDamageModifier(const Wounds &wounds, const Unit *attacker, int hitRoll,
                                                    int woundRoll) const {
        Wounds modifedWounds = Unit::targetAttackDamageModifier(wounds, attacker, hitRoll, woundRoll);

        // Scaly Skin
        if (hasKeyword(COALESCED)) {
            if (modifedWounds.normal > 1) modifedWounds.normal--;
        }
        return modifedWounds;
    }

    int SeraphonBase::woundModifier() const {
        auto mod = Unit::woundModifier();

        // Mighty Beasts of War
        if (hasKeyword(MONSTER) && hasKeyword(THUNDER_LIZARD)) mod += 2;

        return mod;
    }

    void SeraphonBase::setCommandTrait(CommandTrait trait) {
        m_commandTrait = trait;
    }

    void SeraphonBase::setArtefact(Artefact artefact) {
        m_artefact = artefact;
    }

    void Init() {
        DreadSaurian::Init();
        SaurusGuard::Init();
        SaurusKnights::Init();
        SaurusWarriors::Init();
        Skinks::Init();
        SaurusOldblood::Init();
        SaurusSunblood::Init();
        SaurusOldbloodOnCarnosaur::Init();
        SaurusScarVeteranOnCarnosaur::Init();
        Salamanders::Init();
        Razordons::Init();
        Troglodon::Init();
        Bastiladon::Init();
        Kroxigor::Init();
        RipperdactylRiders::Init();
        TerradonRiders::Init();
        Stegadon::Init();
        EngineOfTheGods::Init();
        SaurusScarVeteranOnColdOne::Init();
        ChameleonSkinks::Init();
        TerradonChief::Init();
        RipperdactylChief::Init();
        SaurusAstrolithBearer::Init();
        SaurusEternityWarden::Init();
        LordKroak::Init();
        SkinkStarseer::Init();
        SkinkPriest::Init();
        SkinkStarpriest::Init();
        SlannStarmaster::Init();
    }

} //namespace Seraphon
