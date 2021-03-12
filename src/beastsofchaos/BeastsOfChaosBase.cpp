/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <magic_enum.hpp>
#include <beastsofchaos/BeastsOfChaosBase.h>
#include <Roster.h>
#include <Board.h>
#include "beastsofchaos/Gors.h"
#include "beastsofchaos/Ghorgon.h"
#include "beastsofchaos/Ungors.h"
#include "beastsofchaos/Bestigors.h"
#include "beastsofchaos/Bullgors.h"
#include "beastsofchaos/Cygor.h"
#include "beastsofchaos/Centigors.h"
#include "beastsofchaos/Doombull.h"
#include "beastsofchaos/Chimera.h"
#include "beastsofchaos/Beastlord.h"
#include "beastsofchaos/GreatBrayShaman.h"
#include "beastsofchaos/UngorRaiders.h"
#include "beastsofchaos/TuskgorChariots.h"
#include "beastsofchaos/Cockatrice.h"
#include "beastsofchaos/ChaosSpawn.h"
#include "beastsofchaos/ChaosWarhounds.h"
#include "beastsofchaos/Jabberslythe.h"
#include "beastsofchaos/DragonOgors.h"
#include "beastsofchaos/DragonOgorShaggoth.h"
#include "beastsofchaos/Razorgors.h"

namespace BeastsOfChaos {

    void BeastsOfChaosBase::setGreatfray(Greatfray fray) {
        removeKeyword(ALLHERD);
        removeKeyword(DARKWALKERS);
        removeKeyword(GAVESPAWN);

        m_greatfray = fray;

        switch (fray) {
            case Greatfray::Allherd:
                addKeyword(ALLHERD);
                break;
            case Greatfray::Darkwalkers:
                addKeyword(DARKWALKERS);
                break;
            case Greatfray::Gavespawn:
                addKeyword(GAVESPAWN);
                break;
            default:
                break;
        }
    }

    std::string BeastsOfChaosBase::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Greatfray") {
            auto frayName = magic_enum::enum_name((Greatfray)parameter.intValue);
            return std::string(frayName);
        }
        if (std::string(parameter.name) == "Lore") {
            auto lore = magic_enum::enum_name((Lore)parameter.intValue);
            return std::string(lore);
        }
        if (std::string(parameter.name) == "Command Trait") {
            auto traitName = magic_enum::enum_name((CommandTrait)parameter.intValue);
            return std::string(traitName);
        }
        if (std::string(parameter.name) == "Artefact") {
            auto artefactName = magic_enum::enum_name((Artefact)parameter.intValue);
            return std::string(artefactName);
        }
        return ParameterValueToString(parameter);
    }

    int BeastsOfChaosBase::EnumStringToInt(const std::string &enumString) {
        auto fray = magic_enum::enum_cast<Greatfray>(enumString);
        if (fray.has_value()) return (int)fray.value();

        auto lore = magic_enum::enum_cast<Lore>(enumString);
        if (lore.has_value()) return (int)lore.value();

        auto trait = magic_enum::enum_cast<CommandTrait>(enumString);
        if (trait.has_value()) return (int)trait.value();

        auto artefact = magic_enum::enum_cast<Artefact>(enumString);
        if (artefact.has_value()) return (int)artefact.value();

        return 0;
    }

    void BeastsOfChaosBase::onEndCombat(PlayerId player) {
        Unit::onEndCombat(player);

        // Bloodgorge
        if (m_currentRecord.m_enemyUnitsSlain) {
            heal(Dice::RollD3());
        }

        if (isGeneral() && (m_commandTrait == CommandTrait::Gouge_Tusks)) {
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0);
            for (auto unit : units) {
                if (unit->remainingModels() > 0) {
                    auto roll = Dice::RollD6();
                    if (roll == 6) {
                        unit->applyDamage({0, Dice::RollD3(), Wounds::Source::Ability}, this);
                    }
                    else if (roll >= 2) {
                        unit->applyDamage({0, 1, Wounds::Source::Ability}, this);
                    }
                    break;
                }
            }
        }
    }

    void BeastsOfChaosBase::setArtefact(Artefact artefact) {
        m_artefact = artefact;
    }

    void BeastsOfChaosBase::setCommandTrait(CommandTrait commandTrait) {
        m_commandTrait = commandTrait;
    }

    int BeastsOfChaosBase::woundModifier() const {
        auto mod = Unit::woundModifier();
        if (isGeneral() && (m_commandTrait == CommandTrait::Indomitable_Beast)) {
            mod += 1;
        }
        return mod;
    }

    Rerolls BeastsOfChaosBase::toWoundRerolls(const Weapon *weapon, const Unit *target) const {
        if (isGeneral() && (m_commandTrait == CommandTrait::Apex_Predator)) {
            return Rerolls::Ones;
        }
        if (isGeneral() && (m_commandTrait == CommandTrait::Tempestuous_Tyrant) && target->hasKeyword(MONSTER)) {
            return Rerolls::Failed;
        }
        return Unit::toWoundRerolls(weapon, target);
    }

    int BeastsOfChaosBase::toSaveModifier(const Weapon *weapon, const Unit *attacker) const {
        auto mod = Unit::toSaveModifier(weapon, attacker);
        if (isGeneral() && (m_commandTrait == CommandTrait::Adamantine_Scales)) {
            mod++;
        }
        return mod;
    }

    void BeastsOfChaosBase::onBeginRound(int battleRound) {
        Unit::onBeginRound(battleRound);

        if ((m_battleRound == 1) && isGeneral() && (m_commandTrait == CommandTrait::Ancient_Beyond_Knowledge)) {
            getRoster()->addCommandPoints(Dice::RollD3());
        }
    }

    int BeastsOfChaosBase::targetHitModifier(const Weapon *weapon, const Unit *attacker) const {
        auto mod = Unit::targetHitModifier(weapon, attacker);
        if (isGeneral() && (m_commandTrait == CommandTrait::Shadowpelt) && distanceTo(attacker) > 3.0) {
            mod--;
        }
        return mod;
    }

    Rerolls BeastsOfChaosBase::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        if (isGeneral() && (m_commandTrait == CommandTrait::Eater_Of_Heroes) && target->hasKeyword(HERO)) {
            return Rerolls::Failed;
        }
        return Unit::toHitRerolls(weapon, target);
    }

    int BeastsOfChaosBase::weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        auto rend = Unit::weaponRend(weapon, target, hitRoll, woundRoll);

        if (isGeneral() && (m_commandTrait == CommandTrait::Rugged_Hide)) {
            rend++;
        }
        return rend;
    }

    void Init() {
        Beastlord::Init();
        Bestigors::Init();
        Bullgors::Init();
        Centigors::Init();
        ChaosSpawn::Init();
        ChaosWarhounds::Init();
        Chimera::Init();
        Cockatrice::Init();
        Cygor::Init();
        Doombull::Init();
        DragonOgorShaggoth::Init();
        DragonOgors::Init();
        Ghorgon::Init();
        Gors::Init();
        GreatBrayShaman::Init();
        Jabberslythe::Init();
        Razorgors::Init();
        TuskgorChariots::Init();
        UngorRaiders::Init();
        Ungors::Init();
    }

} // namespace BeastsOfChaos
