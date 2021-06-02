/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/Torgillius.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include <Board.h>
#include "SoulblightGravelordsPrivate.h"
#include "Lore.h"
#include "Roster.h"

namespace Soulblight {
    static const int g_basesize = 40;
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 100;

    bool TorgilliusTheChamberlain::s_registered = false;

    Unit *TorgilliusTheChamberlain::Create(const ParameterList &parameters) {
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_vampireLore[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new TorgilliusTheChamberlain(lore, general);
    }

    int TorgilliusTheChamberlain::ComputePoints(const ParameterList &parameters) {
        return g_pointsPerUnit;
    }

    void TorgilliusTheChamberlain::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SoulblightBase::ValueToString,
                    SoulblightBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Lore", g_vampireLore[0], g_vampireLore),
                            BoolParameter("General")
                    },
                    DEATH,
                    {SOULBLIGHT_GRAVELORDS}
            };
            s_registered = UnitFactory::Register("Torgillius The Chamberlain", factoryMethod);
        }
    }

    TorgilliusTheChamberlain::TorgilliusTheChamberlain(Lore lore, bool isGeneral) :
        SoulblightBase(CursedBloodline::Vyrkos_Dynasty, "Torgillius The Chamberlain", 5, g_wounds, 10, 6, false, g_pointsPerUnit) {

        m_keywords = {DEATH, VAMPIRE, SOULBLIGHT_GRAVELORDS, VYRKOS_DYNASTY, HERO, WIZARD, BELLADAMMA_VOLGA};
        m_weapons = {&m_clawsAndFangs};
        m_battleFieldRole = Role::Leader;
        m_totalSpells = 1;
        m_totalUnbinds = 1;

        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_clawsAndFangs);
        model->setName("Torgillius The Chamberlain");
        addModel(model);

        m_knownSpells.push_back(std::make_unique<BuffModifierSpell>(this, "Necrotising Bolt", 6, 18,
                                                                    std::vector<std::pair<Attribute, int>>{{Attribute::To_Hit_Melee, -1}, {Attribute::To_Hit_Missile, -1}},
                                                                    Abilities::Target::Enemy));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    }

    Wounds TorgilliusTheChamberlain::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        auto totalWounds = SoulblightBase::applyWoundSave(wounds, attackingUnit);
        // Mastery of Grave-sand
        int numSixes = 0;
        return ignoreWounds(totalWounds, 4, numSixes);
    }

    void TorgilliusTheChamberlain::onStartHero(PlayerId player) {
        SoulblightBase::onStartHero(player);

        // Trusted Lieutenant
        if (owningPlayer() == player) {
            auto radukar = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), RADUKAR_THE_WOLF, 3.0);
            if (radukar != nullptr) {
                if (Dice::RollD6() >= 4) {
                    m_roster->addCommandPoints(1);
                }
            }
        }
    }

} // namespace Soulblight
