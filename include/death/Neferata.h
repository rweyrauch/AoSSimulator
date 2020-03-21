/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef NEFERATA_H
#define NEFERATA_H

#include <death/LegionOfNagash.h>
#include <Weapon.h>

namespace Death {

    class NeferataMortarchOfBlood : public LegionOfNagashBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        NeferataMortarchOfBlood();

        ~NeferataMortarchOfBlood() override = default;

        bool configure();

    protected:

        void onWounded() override;

        void onRestore() override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int getDamageTableIndex() const;

    private:

        Weapon m_akmetHar,
            m_akenSeth,
            m_skeletalClaws,
            m_clawsAndDaggers;

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Dagger of Jet                    TODO
// Mortarch of Blood                TODO
// Frightful Touch                  Yes
// Deathly Invocations              TODO
// Dark Mist                        TODO
// Twilight's Allure                TODO

} // namespace Death

#endif // NEFERATA_H