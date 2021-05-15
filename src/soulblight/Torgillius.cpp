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
#include "SoulblightGravelordsPrivate.h"
#include "Lore.h"

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

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    }

} // namespace Soulblight
