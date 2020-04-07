/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef MANNFRED_H
#define MANNFRED_H

#include <death/LegionOfNagash.h>
#include <Weapon.h>

namespace Death {

    class MannfredMortarchOfNight : public LegionOfNagashBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        MannfredMortarchOfNight();

        ~MannfredMortarchOfNight() override = default;

        bool configure();

    protected:

        void onWounded() override;

        void onRestore() override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onStartHero(PlayerId player) override;

        Wounds onEndCombat(PlayerId player) override;

        int getDamageTableIndex() const;

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

} // namespace Death

#endif // MANNFRED_H