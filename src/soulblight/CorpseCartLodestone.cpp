/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/CorpseCartLodestone.h>
#include <UnitFactory.h>
#include "SoulblightGravelordsPrivate.h"

namespace Soulblight {
    static const int g_basesize = 105; // x70 oval
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 80;

    bool CorpseCartWithUnholyLodestone::s_registered = false;

    Unit *CorpseCartWithUnholyLodestone::Create(const ParameterList &parameters) {
        auto bloodline = (CursedBloodline) GetEnumParam("Bloodline", parameters, g_bloodlines[0]);
        return new CorpseCartWithUnholyLodestone(bloodline);
    }

    int CorpseCartWithUnholyLodestone::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void CorpseCartWithUnholyLodestone::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SoulblightBase::ValueToString,
                    SoulblightBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Bloodline", g_bloodlines[0], g_bloodlines)
                    },
                    DEATH,
                    {SOULBLIGHT_GRAVELORDS}
            };
            s_registered = UnitFactory::Register("Corpse Cart with Unholy Lodestone", factoryMethod);
        }
    }

    CorpseCartWithUnholyLodestone::CorpseCartWithUnholyLodestone(CursedBloodline bloodline) :
            SoulblightBase(bloodline, "Corpse Cart with Unholy Lodestone", 4, g_wounds, 10, 6, false, g_pointsPerUnit),
            m_goad(Weapon::Type::Melee, "Corpse Goad", 2, 2, 4, 4, 0, 1),
            m_lash(Weapon::Type::Melee, "Corpse Lash", 1, 3, 4, 4, 0, 1),
            m_blades(Weapon::Type::Melee, "Rusty Blades", 1, RAND_2D6, 5, 5, 0, 1) {
        m_keywords = {DEATH, SOULBLIGHT_GRAVELORDS, DEADWALKERS, CORPSE_CART, CORPSE_CART_WITH_UNHOLY_LODESTONE};
        m_weapons = {&m_goad, &m_lash, &m_blades};
        s_globalCastMod.connect(this, &CorpseCartWithUnholyLodestone::unholyLodestoneCastingMod, &m_lodestoneSlot);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_goad);
        model->addMeleeWeapon(&m_lash);
        model->addMeleeWeapon(&m_blades);
        addModel(model);
    }

    CorpseCartWithUnholyLodestone::~CorpseCartWithUnholyLodestone() {
        m_lodestoneSlot.disconnect();
    }

    int CorpseCartWithUnholyLodestone::unholyLodestoneCastingMod(const Unit *caster) {
        if (isFriendly(caster) && hasKeyword(DEATH) && hasKeyword(WIZARD) && (distanceTo(caster) <= 12.0)) return 1;
        return 0;
    }

} // namespace Soulblight
