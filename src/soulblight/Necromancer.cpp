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
#include "SoulblightGravelordsPrivate.h"
#include "Lore.h"

namespace Soulblight {
    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 125;

    bool Necromancer::s_registered = false;

    Necromancer::Necromancer(Legion legion, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral) :
            SoulblightBase(legion, "Necromancer", 5, g_wounds, 10, 6, false, g_pointsPerUnit),
            m_staff(Weapon::Type::Melee, "Mortis Staff", 1, 2, 3, 3, -1, RAND_D3) {
        m_keywords = {DEATH, NECROMANCER, DEATHMAGES, HERO, WIZARD};
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
        auto legion = (Legion) GetEnumParam("Legion", parameters, g_legions[0]);
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_deathmageLore[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new Necromancer(legion, lore, trait, artefact, general);
    }

    void Necromancer::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SoulblightBase::ValueToString,
                    SoulblightBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Legion", g_legions[0], g_legions),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            EnumParameter("Lore", g_deathmageLore[0], g_deathmageLore),
                            BoolParameter("General")
                    },
                    DEATH,
                    {DEATHMAGES}
            };
            s_registered = UnitFactory::Register("Necromancer", factoryMethod);
        }
    }

    Wounds Necromancer::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        return SoulblightBase::applyWoundSave(wounds, attackingUnit);
    }

    int Necromancer::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void Necromancer::onStartHero(PlayerId player) {
        SoulblightBase::onStartHero(player);

        if (owningPlayer() == player) deathlyInvocations(2, 6.0);
    }

} // namespace Soulblight
