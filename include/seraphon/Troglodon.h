/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon {

    class Troglodon : public SeraphonBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Troglodon();

        ~Troglodon() override;

        bool configure(Lore lore);

    protected:

        void onWounded() override;

        void onRestore() override;

        size_t getDamageTableIndex() const;

        void onStartHero(PlayerId player) override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int castingModifier() const override;

        int terror(const Unit *target);

    private:

        Weapon m_spittle,
                m_jaws,
                m_forelimbs,
                m_rod;

        lsignal::slot m_connection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Oracle of the Slann              Yes
// Regeneration                     Yes
// Terror                           Yes
// Venomous Spittle                 Yes
// Drawn to the Screams             TODO
// Comet's Call                     Yes
//

} // namespace Seraphon
