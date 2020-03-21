/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BLOODSEEKERPALANQUIN_H
#define BLOODSEEKERPALANQUIN_H

#include <death/LegionOfNagash.h>
#include <Weapon.h>

namespace Death {

    class BloodseekerPalanquin : public LegionOfNagashBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        BloodseekerPalanquin();

        ~BloodseekerPalanquin() override = default;

        bool configure();

    protected:

        void onWounded() override;

        void onRestore() override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onStartHero(PlayerId player) override;

        int getDamageTableIndex() const;

    private:

        Weapon m_wail,
            m_blade,
            m_etherealWeapons;

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Frightful Touch                  Yes
// A Fine Vintage                   TODO
// Wail of the Damned               TODO
// Deathly Invocation               Yes
// Blood Siphon                     TODO
//

} // namespace Death

#endif // BLOODSEEKERPALANQUIN_H