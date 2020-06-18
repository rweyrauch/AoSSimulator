/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef MEGABOSS_MAWKRUSHA_H
#define MEGABOSS_MAWKRUSHA_H

#include <ironjawz/Ironjawz.h>

namespace Ironjawz {

    class MegabossOnMawKrusha : public Ironjawz {
    public:

        enum WeaponOption {
            HackaAndChoppa = 0,
            ChoppaAndRiptoofFist,
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        MegabossOnMawKrusha();

        ~MegabossOnMawKrusha() override = default;

        bool configure(WeaponOption weapons, MountTrait trait);

    protected:

        void onWounded() override;

        void onCharged() override;

        void onRestore() override;

        void onStartCombat(PlayerId player) override;

        Wounds onEndCombat(PlayerId player) override;

        Wounds computeReturnedDamage(const Weapon *weapon, int saveRoll) const override;

    private:

        int getDamageTableIndex() const;

        WeaponOption m_weaponOption = HackaAndChoppa;
        int m_modelsSlainAtStartOfCombat = 0;

        Weapon m_bellow,
                m_hackaAndChoppa,
                m_ripToofFist,
                m_fistsAndTail;

        MountTrait m_mountTrait = MountTrait::None;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Destructive Bulk                 Yes
// Rip-toof Fist                    Yes
// Strength from Victory            Yes
// Go on Ladz, Get Stuck In         TODO
//

} // namespace Ironjawz

#endif //MEGABOSS_MAWKRUSHA_H