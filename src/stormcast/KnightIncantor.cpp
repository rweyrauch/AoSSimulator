/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/KnightIncantor.h>
#include <iostream>
#include <spells/MysticShield.h>
#include <stormcast/StormcastSpells.h>
#include "UnitFactory.h"
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int g_basesize = 40;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 120;

    bool KnightIncantor::s_registered = false;

    KnightIncantor::KnightIncantor(Stormhost stormhost, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral) :
            StormcastEternal(stormhost, "Knight-Incantor", 5, g_wounds, 9, 3, false, g_pointsPerUnit),
            m_staff(Weapon::Type::Melee, "Incantor's Staff", 2, 3, 3, 3, -1, RAND_D3) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, SACROSANCT, HERO, WIZARD, KNIGHT_INCANTOR};
        m_weapons = {&m_staff};
        m_battleFieldRole = Role::Leader;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        m_totalSpells = 1;
        m_totalUnbinds = 1;

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_staff);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateSpiritStorm(this)));

        m_points = g_pointsPerUnit;
    }

    Unit *KnightIncantor::Create(const ParameterList &parameters) {
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);
        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, g_stormhost[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTrait[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_celestialMagic[0]);
        auto general = GetBoolParam("General", parameters, false);

        return new KnightIncantor(stormhost, lore, trait, artefact, general);
    }

    void KnightIncantor::Init() {
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

            s_registered = UnitFactory::Register("Knight-Incantor", factoryMethod);
        }
    }

    void KnightIncantor::onStartCombat(PlayerId /*player*/) {
        // Spirit Flask
        if (!m_shatteredFlasks) {
            m_shatteredFlasks = DoSpiritFlasks(this);
        }
    }

    int KnightIncantor::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    int KnightIncantor::unbindingModifier() const {
        if (!m_usedVoidstormScroll) {
            m_usedVoidstormScroll = true;
            return Automatically_Unbound;
        }
        return StormcastEternal::unbindingModifier();
    }

} // namespace StormcastEternals