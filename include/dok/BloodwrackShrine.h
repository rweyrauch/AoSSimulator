/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <dok/DaughterOfKhaine.h>
#include <Weapon.h>

namespace DaughtersOfKhaine {

    class BloodwrackShrine : public DaughterOfKhaine {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        BloodwrackShrine();

        ~BloodwrackShrine() override = default;

    protected:

        void configure(Lore lore);

        void onWounded() override;

        void onRestore() override;

        void onCharged() override;

        void onStartHero(PlayerId player) override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        size_t getDamageTableIndex() const;

        Weapon m_bloodwrackStare,
                m_whisperclaw,
                m_tailOfSerpents,
                m_bloodwrackSpear,
                m_goadstaves;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Bladed Impact                    Yes
// Bloodwrack Stare                 Yes
// Aura of Agony                    Yes
// Enfeebling Foe                   Yes
//

} // namespace DaughtersOfKhaine
