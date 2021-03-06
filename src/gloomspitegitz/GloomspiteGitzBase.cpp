/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <Board.h>
#include "Roster.h"
#include <magic_enum.hpp>
#include <gloomspitegitz/GloomspiteGitzBase.h>
#include <gloomspitegitz/BadMoon.h>
#include "gloomspitegitz/AleguzzlerGargant.h"
#include "gloomspitegitz/ArachnarokSpiderWithFlinger.h"
#include "gloomspitegitz/ArachnarokSpiderWithWarparty.h"
#include "gloomspitegitz/BoingrotBounderz.h"
#include "gloomspitegitz/ColossalSquig.h"
#include "gloomspitegitz/DankholdTroggboss.h"
#include "gloomspitegitz/DankholdTroggoths.h"
#include "gloomspitegitz/FellwaterTroggoths.h"
#include "gloomspitegitz/FungoidCaveShaman.h"
#include "gloomspitegitz/Gobbapalooza.h"
#include "gloomspitegitz/Loonboss.h"
#include "gloomspitegitz/LoonbossOnGiantCaveSquig.h"
#include "gloomspitegitz/LoonbossOnManglerSquigs.h"
#include "gloomspitegitz/LoonbossWithGiantCaveSquig.h"
#include "gloomspitegitz/LoonsmashaFanatics.h"
#include "gloomspitegitz/MadcapShaman.h"
#include "gloomspitegitz/ManglerSquigs.h"
#include "gloomspitegitz/Mollog.h"
#include "gloomspitegitz/RippasSnarlfangs.h"
#include "gloomspitegitz/RockgutTroggoths.h"
#include "gloomspitegitz/ScuttlebossOnSpider.h"
#include "gloomspitegitz/Shootas.h"
#include "gloomspitegitz/SkitterstrandArachnarok.h"
#include "gloomspitegitz/Skragrott.h"
#include "gloomspitegitz/SneakySnufflers.h"
#include "gloomspitegitz/SpiderRiders.h"
#include "gloomspitegitz/SporesplattaFanatics.h"
#include "gloomspitegitz/SquigGobba.h"
#include "gloomspitegitz/SquigHerd.h"
#include "gloomspitegitz/SquigHoppers.h"
#include "gloomspitegitz/Stabbas.h"
#include "gloomspitegitz/TroggothHag.h"
#include "gloomspitegitz/WebspinnerShaman.h"
#include "gloomspitegitz/WebspinnerShamanOnSpider.h"
#include "gloomspitegitz/Zarbag.h"
#include "gloomspitegitz/ZarbagsGitz.h"
#include "../AoSSimPrivate.h"

namespace GloomspiteGitz {

    class ImDaBossNowStabEmGood : public CommandAbility {
    public:
        explicit ImDaBossNowStabEmGood(Unit *source) :
                CommandAbility(source, "I'm Da Boss, Now Stab 'Em Good", 24, 12, GamePhase::Combat) {
            m_allowedTargets = Abilities::Target::SelfAndFriendly;
            m_targetKeywords = {MOONCLAN, GROT};
            m_effect = Abilities::EffectType::Buff;
        }

    protected:

        bool apply(Unit *target) override {
            if (target == nullptr)
                return false;

            target->buffAbility(Ability::Extra_Mortal_Wound_On_Hit_Roll, 6,
                                {GamePhase::Combat, m_round, m_source->owningPlayer()});

            return true;
        }

        bool apply(double x, double y) override { return false; }
    };

    CommandAbility *CreateImDaBossNoStabEmGood(Unit *source) {
        return new ImDaBossNowStabEmGood(source);
    }

    std::string GloomspiteGitzBase::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Lore") {
            return ToString((Lore) parameter.intValue);
        }
        if (std::string(parameter.name) == "Command Trait") {
            auto traitName = magic_enum::enum_name((CommandTrait) parameter.intValue);
            return std::string(traitName);
        }
        if (std::string(parameter.name) == "Artefact") {
            auto artefactName = magic_enum::enum_name((Artefact) parameter.intValue);
            return std::string(artefactName);
        }
        return ParameterValueToString(parameter);
    }

    int GloomspiteGitzBase::EnumStringToInt(const std::string &enumString) {
        Lore lore;
        if (FromString(enumString, lore)) return ToInteger(lore);

        auto trait = magic_enum::enum_cast<CommandTrait>(enumString);
        if (trait.has_value()) return (int) trait.value();

        auto artefact = magic_enum::enum_cast<Artefact>(enumString);
        if (artefact.has_value()) return (int) artefact.value();

        return 0;
    }

    void GloomspiteGitzBase::onBeginRound(int battleRound) {
        Unit::onBeginRound(battleRound);

        // First GG moves the moon
        m_movedMoon = BadMoon::Instance()->move(battleRound);

        if ((battleRound == 1) && isGeneral() &&
            ((m_commandTrait == CommandTrait::Cunning_Plans) || (m_commandTrait == CommandTrait::Low_Cunning))) {
            getRoster()->addCommandPoints(1);
        }
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
                    target->applyDamage({0, mortal}, this);

                    PLOG_INFO.printf("Gloomspite Gitz Bad Moon inflicts %d mortal wounds on %s in round %d.\n",
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
            PLOG_INFO.printf("Gloomspite Gitz wizard, %s, is under the light of the Bad Moon.\n",
                             name().c_str());

            if (isGeneral() && (m_commandTrait == CommandTrait::Loon_Touched)) {
                modifier += 1;
            }
        }

        return modifier;
    }

    Rerolls GloomspiteGitzBase::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Moonclan Fungus Brew
        if (hasKeyword(MOONCLAN) && hasKeyword(GROT) && inLightOfTheBadMoon()) {
            return Rerolls::Ones;
        }
        return Unit::toHitRerolls(weapon, target);
    }

    bool GloomspiteGitzBase::inLightOfTheBadMoon() const {
        const auto moonLocation = BadMoon::Instance()->location();

        if (m_commandTrait == CommandTrait::Loonskin) {
            if (moonLocation != BadMoon::Location::Set) {
                return true;
            }
        }
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
            buffMovement(MovementRule::Run_And_Charge, true, {GamePhase::Hero, m_battleRound + 1, owningPlayer()});
        }
    }

    void GloomspiteGitzBase::setCommandTrait(CommandTrait trait) {
        m_commandTrait = trait;

        if (m_commandTrait == CommandTrait::Great_Shaman) {
            constexpr std::array<Lore, 6> lore = {Lore::Vindictive_Glare, Lore::Itchy_Nuisance,
                                                  Lore::The_Great_Green_Spite, Lore::The_Hand_Of_Gork,
                                                  Lore::Squig_Lure, Lore::Call_Da_Moon};
            // TODO: make sure added spells are unique
            m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore[Dice::RollD6()], this)));
        }
    }

    void GloomspiteGitzBase::setArtefact(Artefact artefact) {
        m_artefact = artefact;
    }

    Rerolls GloomspiteGitzBase::toWoundRerolls(const Weapon *weapon, const Unit *target) const {
        if (isGeneral() && (m_commandTrait == CommandTrait::Sneaky_Stabba) && weapon->isMelee()) {
            return Rerolls::Failed;
        }
        return Unit::toWoundRerolls(weapon, target);
    }

    int GloomspiteGitzBase::woundModifier() const {
        auto mod = Unit::woundModifier();
        if (isGeneral() && ((m_commandTrait == CommandTrait::Tough_N_Leathery) ||
                            (m_commandTrait == CommandTrait::Tough_As_Rocks))) {
            mod += 2;
        }
        return mod;
    }

    int GloomspiteGitzBase::moveModifier() const {
        auto mod = Unit::moveModifier();
        if (isGeneral() && (m_commandTrait == CommandTrait::Master_Spider_Rider)) {
            mod += 4;
        }
        return mod;
    }

    void GloomspiteGitzBase::setAllegiance(Allegiance allegiance) {
        removeKeyword(JAWS_OF_MORK);
        removeKeyword(GLOGGS_MEGAMOB);
        removeKeyword(GRIMSCUTTLE);

        m_allegiance = allegiance;
        switch (m_allegiance) {
            case Allegiance::Jaws_of_Mork:
                addKeyword(JAWS_OF_MORK);
                break;
            case Allegiance::Gloggs_Megamob:
                addKeyword(GLOGGS_MEGAMOB);
                break;
            case Allegiance::Grimscuttle_Tribes:
                addKeyword(GRIMSCUTTLE);
                break;
            default:
                break;
        }
    }

    void Init() {
        AleguzzlerGargant::Init();
        ArachnarokSpiderWithFlinger::Init();
        ArachnarokSpiderWithSpiderfangWarparty::Init();
        BoingrotBounderz::Init();
        ColossalSquig::Init();
        DankholdTroggboss::Init();
        DankholdTroggoths::Init();
        FellwaterTroggoths::Init();
        FungoidCaveShaman::Init();
        Gobbapalooza::Init();
        Loonboss::Init();
        LoonbossOnGiantCaveSquig::Init();
        LoonbossOnManglerSquigs::Init();
        LoonbossWithGiantCaveSquig::Init();
        LoonsmashaFanatics::Init();
        MadcapShaman::Init();
        ManglerSquigs::Init();
        Mollog::Init();
        RippasSnarlfangs::Init();
        RockgutTroggoths::Init();
        ScuttlebossOnGiganticSpider::Init();
        Shootas::Init();
        SkitterstrandArachnarok::Init();
        Skragrott::Init();
        SneakySnufflers::Init();
        SpiderRiders::Init();
        SporesmashaFanatics::Init();
        SquigGobba::Init();
        SquiqHerd::Init();
        SquiqHoppers::Init();
        Stabbas::Init();
        TroggothHag::Init();
        WebspinnerShaman::Init();
        WebspinnerShamanOnArachnarokSpider::Init();
        Zarbag::Init();
        ZarbagsGitz::Init();
    }

} //namespace GloomspiteGitz

