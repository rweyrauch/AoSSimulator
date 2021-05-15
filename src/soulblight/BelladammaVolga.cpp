/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/BelladammaVolga.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "SoulblightGravelordsPrivate.h"
#include "Lore.h"

namespace Soulblight {
    static const int g_basesize = 50;
    static const int g_wounds = 9;
    static const int g_pointsPerUnit = 0;

    bool BelladammaVolga::s_registered = false;

    Unit *BelladammaVolga::Create(const ParameterList &parameters) {
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_vampireLore[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new BelladammaVolga(lore, general);
    }

    int BelladammaVolga::ComputePoints(const ParameterList &parameters) {
        return g_pointsPerUnit;
    }

    void BelladammaVolga::Init() {
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
            s_registered = UnitFactory::Register("Belladamma Volga", factoryMethod);
        }
    }

    BelladammaVolga::BelladammaVolga(Lore lore, bool isGeneral) :
            SoulblightBase(CursedBloodline::Vyrkos_Dynasty, "Belladamma Volga", 10, g_wounds, 10, 4, false, g_pointsPerUnit) {

        m_keywords = {DEATH, VAMPIRE, SOULBLIGHT_GRAVELORDS, VYRKOS_DYNASTY, HERO, WIZARD, BELLADAMMA_VOLGA};
        m_weapons = {&m_scimatar, &m_fangsAndClaws};
        m_battleFieldRole = Role::Leader;
        m_totalSpells = 2;
        m_totalUnbinds = 2;

        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_scimatar);
        model->addMeleeWeapon(&m_fangsAndClaws);
        model->setName("Belladamma Volga");
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    }

} // namespace Soulblight
