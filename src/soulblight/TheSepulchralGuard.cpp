/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/TheSepulchralGuard.h>
#include <UnitFactory.h>
#include "SoulblightGravelordsPrivate.h"

namespace Soulblight {
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_pointsPerUnit = 80;

    bool TheSepulchralGuard::s_registered = false;

    Unit *TheSepulchralGuard::Create(const ParameterList &parameters) {
        auto bloodline = (CursedBloodline) GetEnumParam("Bloodline", parameters, g_bloodlines[0]);
        return new TheSepulchralGuard(bloodline);
    }

    int TheSepulchralGuard::ComputePoints(const ParameterList &parameters) {
        return g_pointsPerUnit;
    }

    void TheSepulchralGuard::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SoulblightBase::ValueToString,
                    SoulblightBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Bloodline", g_bloodlines[0], g_bloodlines),
                    },
                    DEATH,
                    {SOULBLIGHT_GRAVELORDS}
            };
            s_registered = UnitFactory::Register("The Sepulchral Guard", factoryMethod);
        }
    }

    TheSepulchralGuard::TheSepulchralGuard(CursedBloodline bloodline):
        SoulblightBase(bloodline, "The Sepulchral Guard", 4, g_wounds, 10, 6, false, g_pointsPerUnit) {

        m_keywords = {DEATH, SOULBLIGHT_GRAVELORDS, DEATHRATTLE, DEATHRATTLE_SKELETONS, THE_SEPULCHRAL_GUARD};
        m_weapons = {&m_spear, &m_mace, &m_greatblade, &m_blade, &m_scythe};

        auto warden = new Model(g_basesize, wounds()+1);
        warden->addMeleeWeapon(&m_spear);
        warden->setName("Sepulchral Warden");
        addModel(warden);

        auto prince = new Model(g_basesize, wounds());
        prince->addMeleeWeapon(&m_mace);
        prince->setName("Prince of Dust");
        addModel(prince);

        auto champion = new Model(g_basesize, wounds());
        champion->addMeleeWeapon(&m_greatblade);
        champion->setName("Champion");
        addModel(champion);

        auto harvester = new Model(g_basesize, wounds());
        harvester->addMeleeWeapon(&m_scythe);
        harvester->setName("Harvester");
        addModel(harvester);

        for (auto i = 0; i < 3; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_blade);
            addModel(model);
        }
    }

    int TheSepulchralGuard::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        auto hit = SoulblightBase::generateHits(unmodifiedHitRoll, weapon, unit);
        // Serve in Death
        if (unmodifiedHitRoll == 6) {
            hit++;
        }
        return hit;
    }

    void TheSepulchralGuard::onStartHero(PlayerId player) {
        SoulblightBase::onStartHero(player);

        if (owningPlayer() == player) {
            if (isNamedModelAlive("Sepulchral Warden")) {
                restoreModels(Dice::RollD3());
            }
        }
    }

} // namespace Soulblight
