/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <magic_enum.hpp>
#include <beastsofchaos/BeastsOfChaosBase.h>
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
    }

    void BeastsOfChaosBase::setArtefact(Artefact artefact) {
        m_artefact = artefact;
    }

    void BeastsOfChaosBase::setCommandTrait(CommandTrait commandTrait) {
        m_commandTrait = commandTrait;
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
