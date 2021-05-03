/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <gloomspitegitz/DankholdTroggboss.h>
#include <UnitFactory.h>
#include <Board.h>
#include <Roster.h>
#include "GloomspitePrivate.h"

namespace GloomspiteGitz {

    class InstinctiveLeader : public CommandAbility {
    public:
        explicit InstinctiveLeader(Unit *source);

    protected:
        bool apply(Unit *target) override;

        bool apply(double x, double y) override { return false; }
    };

    InstinctiveLeader::InstinctiveLeader(Unit *source) :
            CommandAbility(source, "Instinctive Leader", 18, 18, GamePhase::Combat) {
        m_allowedTargets = Abilities::Target::SelfAndFriendly;
        m_effect = Abilities::EffectType::Buff;
        m_targetKeywords.push_back(TROGGOTH);
    }

    bool InstinctiveLeader::apply(Unit *target) {

        auto units = Board::Instance()->getUnitsWithin(m_source->position(), m_source->owningPlayer(), m_rangeGeneral);
        for (auto unit : units) {
            if (unit->hasKeyword(TROGGOTH)) {
                unit->buffReroll(Attribute::To_Hit_Melee, Rerolls::Ones, defaultDuration());
            }
        }
        return true;
    }

    static const int g_basesize = 60;
    static const int g_wounds = 12;
    static const int g_pointsPerUnit = 250;

    bool DankholdTroggboss::s_registered = false;

    DankholdTroggboss::DankholdTroggboss(CommandTrait trait, Artefact artefact, bool isGeneral) :
            GloomspiteGitzBase("Dankhold Troggboss", 6, g_wounds, 7, 4, false, g_pointsPerUnit),
            m_boulderClub(Weapon::Type::Melee, "Boulder Club", 2, 4, 3, 3, -2, RAND_D6) {
        m_keywords = {DESTRUCTION, TROGGOTH, GLOOMSPITE_GITZ, DANKHOLD, HERO, TROGGBOSS};
        m_weapons = {&m_boulderClub};
        m_battleFieldRole = Role::Leader;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        s_globalBraveryMod.connect(this, &DankholdTroggboss::reassuringPresence, &m_connection);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_boulderClub);
        addModel(model);

        m_commandAbilities.push_back(std::make_unique<InstinctiveLeader>(this));
    }

    DankholdTroggboss::~DankholdTroggboss() {
        m_connection.disconnect();
    }

    Unit *DankholdTroggboss::Create(const ParameterList &parameters) {
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_fortuitousTroggbossTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_glintyGubbinzThatTroggothsFound[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new DankholdTroggboss(trait, artefact, general);
    }

    void DankholdTroggboss::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    GloomspiteGitzBase::ValueToString,
                    GloomspiteGitzBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Command Trait", g_fortuitousTroggbossTraits[0], g_fortuitousTroggbossTraits),
                            EnumParameter("Artefact", g_glintyGubbinzThatTroggothsFound[0],
                                          g_glintyGubbinzThatTroggothsFound),
                            BoolParameter("General")
                    },
                    DESTRUCTION,
                    {GLOOMSPITE_GITZ}
            };

            s_registered = UnitFactory::Register("Dankhold Troggboss", factoryMethod);
        }
    }

    void DankholdTroggboss::onStartHero(PlayerId player) {
        if (player == owningPlayer()) {
            if (remainingWounds() < g_wounds && remainingWounds() > 0) {
                // Regeneration - heal D3
                // Troggoth Renewal
                if (Dice::RollD6() >= 4 || (inLightOfTheBadMoon() && (Dice::RollD6() >= 4))) {
                    int woundsHealed = Dice::RollD3();
                    if (inLightOfTheBadMoon())
                        woundsHealed *= 2;
                    for (auto &m : m_models) {
                        m->applyHealing(woundsHealed);
                    }
                }
            }
        }
    }

    void DankholdTroggboss::onStartCombat(PlayerId player) {
        if (m_meleeTarget) {
            // Crushing Grip
            int roll = Dice::RollD6();
            if (roll >= m_meleeTarget->wounds()) {
                m_meleeTarget->slay(1);
            } else if (m_commandTrait == CommandTrait::Pulverising_Grip) {
                roll = Dice::RollD6();
                if (roll >= m_meleeTarget->wounds()) {
                    m_meleeTarget->slay(1);
                }
            }
        }

        // Squiggly-beast Followers
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0);
        for (auto ip : units) {
            int roll = Dice::RollD6();
            if (roll >= ip->remainingModels()) {
                ip->applyDamage({0, 1}, this);
            }
        }

        GloomspiteGitzBase::onStartCombat(player);
    }

    int DankholdTroggboss::reassuringPresence(const Unit *unit) {
        // Reassuring Presence
        if (unit->hasKeyword(GLOOMSPITE_GITZ) && (unit->owningPlayer() == owningPlayer()) &&
            (distanceTo(unit) <= 18.0)) {
            if (m_commandTrait == CommandTrait::Alpha_Trogg) {
                return 2;
            }
            return 1;
        }

        return 0;
    }

    int DankholdTroggboss::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    Wounds DankholdTroggboss::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        // Magical Resistance
        if (wounds.source == Wounds::Source::Spell) {
            if (Dice::RollD6() >= 4) {
                return {0, 0, Wounds::Source::Spell};
            } else if (m_commandTrait == CommandTrait::Realmstone_Studded_Hide) {
                if (Dice::RollD6() >= 4) {
                    return {0, 0, Wounds::Source::Spell};
                }
            }
        }
        return Unit::applyWoundSave(wounds, attackingUnit);
    }

} // namespace GloomspiteGitz

