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
    static const int BASESIZE = 60;
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 250;

    bool DankholdTroggboss::s_registered = false;

    DankholdTroggboss::DankholdTroggboss() :
            GloomspiteGitzBase("Dankhold Troggboss", 6, WOUNDS, 7, 4, false),
            m_boulderClub(Weapon::Type::Melee, "Boulder Club", 2, 4, 3, 3, -2, RAND_D6) {
        m_keywords = {DESTRUCTION, TROGGOTH, GLOOMSPITE_GITZ, DANKHOLD, HERO, TROGGBOSS};
        m_weapons = {&m_boulderClub};
        m_battleFieldRole = Leader;

        s_globalBraveryMod.connect(this, &DankholdTroggboss::reassuringPresence, &m_connection);
    }

    DankholdTroggboss::~DankholdTroggboss() {
        m_connection.disconnect();
    }

    bool DankholdTroggboss::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_boulderClub);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *DankholdTroggboss::Create(const ParameterList &parameters) {
        auto unit = new DankholdTroggboss();

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_fortuitousTroggbossTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_glintyGubbinzThatTroggothsFound[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
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
                            EnumParameter("Artefact", g_glintyGubbinzThatTroggothsFound[0], g_glintyGubbinzThatTroggothsFound),
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
            if (remainingWounds() < WOUNDS && remainingWounds() > 0) {
                // Regeneration - heal D3
                // Troggoth Renewal
                if (Dice::rollD6() >= 4 || (inLightOfTheBadMoon() && (Dice::rollD6() >= 4))) {
                    int woundsHealed = Dice::rollD3();
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
            int roll = Dice::rollD6();
            if (roll >= m_meleeTarget->wounds()) {
                m_meleeTarget->slay(1);
            }
        }

        // Squiggly-beast Followers
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0);
        for (auto ip : units) {
            int roll = Dice::rollD6();
            if (roll >= ip->remainingModels()) {
                ip->applyDamage({0, 1});
            }
        }

        GloomspiteGitzBase::onStartCombat(player);
    }

    int DankholdTroggboss::reassuringPresence(const Unit *unit) {
        // Reassuring Presence
        if (unit->hasKeyword(GLOOMSPITE_GITZ) && (unit->owningPlayer() == owningPlayer()) &&
            (distanceTo(unit) <= 18.0)) {
            return 1;
        }

        return 0;
    }

    int DankholdTroggboss::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

    Wounds DankholdTroggboss::applyWoundSave(const Wounds &wounds) {
        // Magical Resistance
        if (wounds.source == Wounds::Source::Spell) {
            if (Dice::rollD6() >= 4) {
                return {0, 0, Wounds::Source::Spell};
            }
        }
        return Unit::applyWoundSave(wounds);
    }

} // namespace GloomspiteGitz

