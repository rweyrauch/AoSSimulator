/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <death/Necromancer.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "LegionOfNagashPrivate.h"

namespace Death {
    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 130;

    bool Necromancer::s_registered = false;

    Necromancer::Necromancer() :
            LegionOfNagashBase("Necromancer", 5, g_wounds, 10, 6, false),
            m_staff(Weapon::Type::Melee, "Necromancer's Staff", 2, 1, 4, 3, -1, RAND_D3) {
        m_keywords = {DEATH, NECROMANCER, DEATHMAGES, HERO, WIZARD};
        m_weapons = {&m_staff};
        m_battleFieldRole = Leader;
        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    bool Necromancer::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_staff);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *Necromancer::Create(const ParameterList &parameters) {
        auto unit = new Necromancer();

        auto legion = (Legion) GetEnumParam("Legion", parameters, g_legions[0]);
        unit->setLegion(legion);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_deathmageLore[0]);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Necromancer::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LegionOfNagashBase::ValueToString,
                    LegionOfNagashBase::EnumStringToInt,
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

    Wounds Necromancer::applyWoundSave(const Wounds &wounds, Unit* attackingUnit) {
        return Unit::applyWoundSave(wounds, attackingUnit);
    }

    int Necromancer::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    void Necromancer::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        if (owningPlayer() == player) deathlyInvocations(2, 6.0);
    }

} // namespace Death
