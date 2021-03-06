/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/Necromancer.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include <Board.h>
#include "SoulblightGravelordsPrivate.h"
#include "Lore.h"

namespace Soulblight {
    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 125;

    bool Necromancer::s_registered = false;

    Necromancer::Necromancer(CursedBloodline bloodline, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral) :
            SoulblightBase(bloodline, "Necromancer", 5, g_wounds, 10, 6, false, g_pointsPerUnit),
            m_staff(Weapon::Type::Melee, "Mortis Staff", 1, 2, 3, 3, -1, RAND_D3) {
        m_keywords = {DEATH, SOULBLIGHT_GRAVELORDS, NECROMANCER, DEATHMAGES, HERO, WIZARD};
        m_weapons = {&m_staff};
        m_battleFieldRole = Role::Leader;
        m_totalSpells = 1;
        m_totalUnbinds = 1;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_staff);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    }

    Unit *Necromancer::Create(const ParameterList &parameters) {
        auto bloodline = (CursedBloodline) GetEnumParam("Bloodline", parameters, g_bloodlines[0]);
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_deathmageLore[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new Necromancer(bloodline, lore, trait, artefact, general);
    }

    void Necromancer::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SoulblightBase::ValueToString,
                    SoulblightBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Bloodline", g_bloodlines[0], g_bloodlines),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            EnumParameter("Lore", g_deathmageLore[0], g_deathmageLore),
                            BoolParameter("General")
                    },
                    DEATH,
                    {SOULBLIGHT_GRAVELORDS}
            };
            s_registered = UnitFactory::Register("Necromancer", factoryMethod);
        }
    }

    Wounds Necromancer::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        auto totalWounds = SoulblightBase::applyWoundSave(wounds, attackingUnit);
        auto summonables = Board::Instance()->getUnitsWithin(this, owningPlayer(), 3.0);
        const Wounds OneMortalWound{0, 1, wounds.source, wounds.sourceObject};
        const Wounds OneNormalWound{1, 0, wounds.source, wounds.sourceObject};
        for (auto unit : summonables) {
            if (unit->hasKeyword(SOULBLIGHT_GRAVELORDS) || unit->hasKeyword(SUMMONABLE)) {
                while ((unit->remainingModels() > 0) && (totalWounds.mortal > 0)) {
                    if (Dice::RollD6() >= 3) {
                        unit->applyDamage(OneMortalWound, this);
                        totalWounds.mortal--;
                    }
                }
                while ((unit->remainingModels() > 0) && (totalWounds.normal > 0)) {
                    if (Dice::RollD6() >= 3) {
                        unit->applyDamage(OneNormalWound, this);
                        totalWounds.normal--;
                    }
                }
            }
        }
        return totalWounds;
    }

    int Necromancer::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace Soulblight
