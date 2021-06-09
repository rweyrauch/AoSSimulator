/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/KnightArcanum.h>
#include <iostream>
#include <spells/MysticShield.h>
#include <stormcast/StormcastSpells.h>
#include "UnitFactory.h"
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int g_basesize = 40;
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 0;

    bool KnightArcanum::s_registered = false;

    KnightArcanum::KnightArcanum(Stormhost stormhost, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral) :
            StormcastEternal(stormhost, "Knight-Arcanum", 5, g_wounds, 9, 3, false, g_pointsPerUnit),
            m_stave(Weapon::Type::Melee, "Stave", 2, 4, 3, 3, -1, RAND_D3) {
        m_keywords = {ORDER, CELESTIAL, THUNDERSTRIKE, STORMCAST_ETERNAL, HERO, WIZARD, LORD_ARCANUM};
        m_weapons = {&m_stave};
        m_battleFieldRole = Role::Leader;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        m_totalSpells = 1;
        m_totalUnbinds = 1;

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_stave);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreatePrimeElectrids(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;
    }

    Unit *KnightArcanum::Create(const ParameterList &parameters) {
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);
        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, g_stormhost[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTrait[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_celestialMagic[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new KnightArcanum(stormhost, lore, trait, artefact, general);
    }

    void KnightArcanum::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    StormcastEternal::ValueToString,
                    StormcastEternal::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Lore", g_lore[0], g_lore),
                            EnumParameter("Stormhost", g_stormhost[0], g_stormhost),
                            EnumParameter("Artefact", g_celestialMagic[0], g_celestialMagic),
                            EnumParameter("Command Trait", g_commandTrait[0], g_commandTrait),
                            BoolParameter("General")
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Knight-Arcanum", factoryMethod);
        }
    }

    int KnightArcanum::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace StormcastEternals