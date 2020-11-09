/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <Board.h>
#include <gloomspitegitz/GloomspiteGitzBase.h>
#include <gloomspitegitz/BadMoon.h>
#include <magic_enum.hpp>
#include "gloomspitegitz/BoingrotBounderz.h"
#include "gloomspitegitz/ColossalSquig.h"
#include "gloomspitegitz/LoonbossOnManglerSquigs.h"
#include "gloomspitegitz/ManglerSquigs.h"
#include "gloomspitegitz/SquigHerd.h"
#include "gloomspitegitz/SquigHoppers.h"
#include "gloomspitegitz/Stabbas.h"
#include "gloomspitegitz/Shootas.h"
#include "gloomspitegitz/RockgutTroggoths.h"
#include "gloomspitegitz/DankholdTroggoths.h"
#include "gloomspitegitz/DankholdTroggboss.h"
#include "gloomspitegitz/Skragrott.h"
#include "gloomspitegitz/Loonboss.h"
#include "gloomspitegitz/LoonbossOnGiantCaveSquig.h"
#include "gloomspitegitz/LoonbossWithGiantCaveSquig.h"
#include "gloomspitegitz/AleguzzlerGargant.h"
#include "gloomspitegitz/SquigGobba.h"
#include "gloomspitegitz/SpiderRiders.h"
#include "gloomspitegitz/FellwaterTroggoths.h"
#include "gloomspitegitz/FungoidCaveShaman.h"
#include "gloomspitegitz/MadcapShaman.h"
#include "gloomspitegitz/WebspinnerShaman.h"
#include "gloomspitegitz/WebspinnerShamanOnSpider.h"
#include "gloomspitegitz/ArachnarokSpiderWithFlinger.h"
#include "gloomspitegitz/ArachnarokSpiderWithWarparty.h"
#include "gloomspitegitz/SkitterstrandArachnarok.h"
#include "gloomspitegitz/SneakySnufflers.h"
#include "gloomspitegitz/ScuttlebossOnSpider.h"
#include "gloomspitegitz/RippasSnarlfangs.h"
#include "gloomspitegitz/Zarbag.h"
#include "gloomspitegitz/ZarbagsGitz.h"
#include "gloomspitegitz/LoonsmashaFanatics.h"
#include "gloomspitegitz/SporesplattaFanatics.h"
#include "gloomspitegitz/Mollog.h"
#include "gloomspitegitz/Gobbapalooza.h"
#include "gloomspitegitz/TroggothHag.h"
#include "../AoSSimPrivate.h"

namespace GloomspiteGitz {

    std::string GloomspiteGitzBase::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Lore") {
            return ToString((Lore) parameter.intValue);
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

    int GloomspiteGitzBase::EnumStringToInt(const std::string &enumString) {
        Lore lore;
        if (FromString(enumString, lore)) return ToInteger(lore);

        auto trait = magic_enum::enum_cast<CommandTrait>(enumString);
        if (trait.has_value()) return (int)trait.value();

        auto artefact = magic_enum::enum_cast<Artefact>(enumString);
        if (artefact.has_value()) return (int)artefact.value();

        return 0;
    }

    void GloomspiteGitzBase::onBeginRound(int battleRound) {
        Unit::onBeginRound(battleRound);

        // First GG moves the moon
        m_movedMoon = BadMoon::Instance()->move(battleRound);
    }

    void GloomspiteGitzBase::onEndRound(int battleRound) {
        Unit::onEndRound(battleRound);

        m_movedMoon = false;
    }

    void GloomspiteGitzBase::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        if (player == owningPlayer()) {
            if (m_movedMoon) {
                std::vector<Unit *> units;
                // Fangs of the Bad Moon
                const auto moonLocation = BadMoon::Instance()->location();
                switch (moonLocation) {
                    case BadMoon::Location::All:
                        units = Board::Instance()->getAllUnits(GetEnemyId(owningPlayer()));
                        break;
                    case BadMoon::Location::Northeast:
                        units = Board::Instance()->getUnitWithin(Board::Northeast, GetEnemyId(owningPlayer()));
                        break;
                    case BadMoon::Location::Northwest:
                        units = Board::Instance()->getUnitWithin(Board::Northwest, GetEnemyId(owningPlayer()));
                        break;
                    case BadMoon::Location::Southeast:
                        units = Board::Instance()->getUnitWithin(Board::Southeast, GetEnemyId(owningPlayer()));
                        break;
                    case BadMoon::Location::Southwest:
                        units = Board::Instance()->getUnitWithin(Board::Southwest, GetEnemyId(owningPlayer()));
                        break;
                    default:
                        // No affect
                        break;
                }

                // Find the strongest unit - maximum points value
                int points = INT32_MIN;
                Unit *target = nullptr;
                for (auto ip : units) {
                    if ((*ip).points() > points) {
                        target = ip;
                        points = target->points();
                    }
                }
                if (target) {
                    int mortal = Dice::RollD3();
                    target->applyDamage({0, mortal});

                    SimLog(Verbosity::Narrative,
                           "Gloomspite Gitz Bad Moon inflicts %d mortal wounds on %s in round %d.\n",
                           mortal, target->name().c_str(), m_battleRound);
                }
            }
        }
    }

    int GloomspiteGitzBase::castingModifier() const {
        int modifier = Unit::castingModifier();

        // Bad Moon Magic
        if (hasKeyword(WIZARD) && inLightOfTheBadMoon()) {
            modifier += 1;
            SimLog(Verbosity::Narrative, "Gloomspite Gitz wizard, %s, is under the light of the Bad Moon.\n",
                   name().c_str());
        }

        return modifier;
    }

    Rerolls GloomspiteGitzBase::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Moonclan Fungus Brew
        if (hasKeyword(MOONCLAN) && hasKeyword(GROT) && inLightOfTheBadMoon()) {
            return Reroll_Ones;
        }
        return Unit::toHitRerolls(weapon, target);
    }

    bool GloomspiteGitzBase::inLightOfTheBadMoon() const {
        const auto moonLocation = BadMoon::Instance()->location();
        switch (moonLocation) {
            case BadMoon::Location::All:
                return true;
            case BadMoon::Location::Northeast:
                return (Board::Instance()->isUnitWithin(Board::Northeast, this));
            case BadMoon::Location::Northwest:
                return (Board::Instance()->isUnitWithin(Board::Northwest, this));
            case BadMoon::Location::Southeast:
                return (Board::Instance()->isUnitWithin(Board::Southeast, this));
            case BadMoon::Location::Southwest:
                return (Board::Instance()->isUnitWithin(Board::Southwest, this));
            default:
                // No affect
                break;
        }
        return false;
    }

    void GloomspiteGitzBase::onRan() {
        Unit::onRan();

        // Lunar Squigs
        if (hasKeyword(SQUIG) && inLightOfTheBadMoon()) {
            buffMovement(Run_And_Charge, true, {Phase::Hero, m_battleRound + 1, owningPlayer()});
        }
    }

    void GloomspiteGitzBase::setCommandTrait(CommandTrait trait) {
        m_commandTrait = trait;
    }

    void GloomspiteGitzBase::setArtefact(Artefact artefact) {
        m_artefact = artefact;
    }

    void Init() {
        BoingrotBounderz::Init();
        ColossalSquig::Init();
        LoonbossOnManglerSquigs::Init();
        ManglerSquigs::Init();
        SquiqHerd::Init();
        SquiqHoppers::Init();
        Stabbas::Init();
        Shootas::Init();
        RockgutTroggoths::Init();
        DankholdTroggboss::Init();
        DankholdTroggoths::Init();
        Skragrott::Init();
        Loonboss::Init();
        LoonbossOnGiantCaveSquig::Init();
        LoonbossWithGiantCaveSquig::Init();
        AleguzzlerGargant::Init();
        SquigGobba::Init();
        SpiderRiders::Init();
        FellwaterTroggoths::Init();
        FungoidCaveShaman::Init();
        MadcapShaman::Init();
        WebspinnerShaman::Init();
        WebspinnerShamanOnArachnarokSpider::Init();
        ArachnarokSpiderWithFlinger::Init();
        ArachnarokSpiderWithSpiderfangWarparty::Init();
        SkitterstrandArachnarok::Init();
        ScuttlebossOnGiganticSpider::Init();
        SneakySnufflers::Init();
        RippasSnarlfangs::Init();
        Zarbag::Init();
        ZarbagsGitz::Init();
        LoonsmashaFanatics::Init();
        SporesmashaFanatics::Init();
        Mollog::Init();
        Gobbapalooza::Init();
        TroggothHag::Init();
    }

} //namespace GloomspiteGitz

