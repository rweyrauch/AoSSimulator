/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <soulblight/SoulblightGravelords.h>
#include <Weapon.h>

namespace Soulblight {

    class MortisEngine : public SoulblightBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        explicit MortisEngine(CursedBloodline bloodline);

        ~MortisEngine() override;

    protected:

        void onWounded() override;

        void onRestore() override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onStartShooting(PlayerId player) override;

        size_t getDamageTableIndex() const;

        void onStartHero(PlayerId player) override;

        int boundNecromancer(const Unit *caster);

    private:

        bool m_usedReliquary = false;

        lsignal::slot m_boundNecromancer;

    private:

        Weapon  m_wail{Weapon::Type::Missile, "Wail of the Damned", 12, 1, 0, 0, 0, 0},
                m_staff{Weapon::Type::Melee, "Mortis Staff", 1, 2, 4, 3, -1, RAND_D3},
                m_etherealWeapons{Weapon::Type::Melee, "Spectral Claws and Blades", 1, 12, 5, 4, 0, 1};

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Wail of the Damned               Yes
// Frightful Touch                  Yes
// The Reliquary                    Yes
// Bound Necromancer                Yes
//

} // namespace Soulblight
