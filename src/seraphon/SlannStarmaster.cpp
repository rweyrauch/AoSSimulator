/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/SlannStarmaster.h>
#include <UnitFactory.h>
#include <Roster.h>
#include <spells/MysticShield.h>
#include <Board.h>
#include "SeraphonPrivate.h"
#include "SeraphonLore.h"

namespace Seraphon {
    static const int g_basesize = 50;
    static const int g_wounds = 7;
    static const int g_pointsPerUnit = 260;

    bool SlannStarmaster::s_registered = false;

    SlannStarmaster::SlannStarmaster(WayOfTheSeraphon way, Constellation constellation, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral) :
            SeraphonBase("Slann Starmaster", 5, g_wounds, 9, 4, true, g_pointsPerUnit) {
        m_keywords = {ORDER, SERAPHON, SLANN, HERO, WIZARD, STARMASTER};
        m_weapons = {&m_lightning};
        m_battleFieldRole = Role::Leader;

        m_totalSpells = 3;
        m_totalUnbinds = 3;

        setWayOfTheSeraphon(way, constellation);
        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_lightning);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateCometsCall(this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_commandAbilities.push_back(std::unique_ptr<CommandAbility>(CreateGiftFromTheHeavens(this)));
    }

    Unit *SlannStarmaster::Create(const ParameterList &parameters) {
        auto way = (WayOfTheSeraphon) GetEnumParam("Way of the Seraphon", parameters, g_wayOfTheSeraphon[0]);
        auto constellation = (Constellation) GetEnumParam("Constellation", parameters, g_constellation[0]);
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_loreOfCelestialDomination[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_slannCommandTrait[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_treasuresOfTheOldOnes[0]);
        auto general = GetBoolParam("General", parameters, false);

        return new SlannStarmaster(way, constellation, lore, trait, artefact, general);
    }

    void SlannStarmaster::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SeraphonBase::ValueToString,
                    SeraphonBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Way of the Seraphon", g_wayOfTheSeraphon[0], g_wayOfTheSeraphon),
                            EnumParameter("Constellation", g_constellation[0], g_constellation),
                            EnumParameter("Command Trait", g_slannCommandTrait[0], g_slannCommandTrait),
                            EnumParameter("Artefact", g_treasuresOfTheOldOnes[0], g_treasuresOfTheOldOnes),
                            EnumParameter("Lore", g_loreOfCelestialDomination[0], g_loreOfCelestialDomination),
                            BoolParameter("General")
                    },
                    ORDER,
                    {SERAPHON}
            };

            s_registered = UnitFactory::Register("Slann Starmaster", factoryMethod);
        }
    }

    void SlannStarmaster::onStartHero(PlayerId player) {
        SeraphonBase::onStartHero(player);

        // Foresight
        if (owningPlayer() == player) {
            Dice::RollResult result;
            Dice::RollD6(2, result);
            m_roster->addCommandPoints(result.rollsGE(4));
        }
    }

    int SlannStarmaster::castingModifier() const {
        auto mod = SeraphonBase::castingModifier();

        // Masters of Order
        mod++;

        return mod;
    }

    int SlannStarmaster::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    Wounds SlannStarmaster::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        // Selfless Protector (Saurus Guard and Saurus Eternity Warden abilities)
        auto guards = Board::Instance()->getUnitsWithin(this, owningPlayer(), 3.0);
        for (auto guard : guards) {
            if (guard->hasKeyword(SAURUS_GUARD) || guard->hasKeyword(ETERNITY_WARDEN)) {
                if (guard->remainingModels() > 0) {
                    if (Dice::RollD6() >= 2) {
                        guard->applyDamage(wounds, this);
                        return {0, 0};
                    }
                }
            }
        }
        return SeraphonBase::applyWoundSave(wounds, attackingUnit);
    }

} //namespace Seraphon
