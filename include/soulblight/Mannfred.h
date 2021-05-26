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

    class MannfredMortarchOfNight : public SoulblightBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        explicit MannfredMortarchOfNight(bool isGeneral);

        ~MannfredMortarchOfNight() override = default;

    protected:

        void onWounded() override;

        void onRestore() override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onStartHero(PlayerId player) override;

        void onEndCombat(PlayerId player) override;

        size_t getDamageTableIndex() const;

    private:

        Weapon m_gheistvor,
                m_glaive,
                m_ebonClaws,
                m_clawsAndDaggers;

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Feaster of Souls                 Yes
// Armour of Templehof              TODO
// Sword of Unholy Power            TODO
// Mortarch of Night                TODO
// Frightful Touch                  Yes
// Deathly Invocation               Yes
// Wind of Death                    TODO
// Vigour of Undeath                TODO
//

} // namespace Soulblight
