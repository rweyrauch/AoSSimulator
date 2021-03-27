/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

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

    protected:

        void configure(Lore lore);

        void onWounded() override;

        void onRestore() override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onStartHero(PlayerId player) override;

        void onEndCombat(PlayerId player) override;

        size_t getDamageTableIndex() const;

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
// Mortarch of Blood                Partial/TODO
// Frightful Touch                  Yes
// Deathly Invocations              Yes
// Dark Mist                        TODO
// Twilight's Allure                TODO

} // namespace Death
