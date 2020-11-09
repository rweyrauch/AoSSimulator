/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef MOURNFANGPACK_H
#define MOURNFANGPACK_H

#include <mawtribes/MawtribesBase.h>
#include <Weapon.h>

namespace OgorMawtribes {

    class MournfangPack : public MawtribesBase {
    public:

        enum WeaponOption {
            Culling_Club_Or_Prey_Hacker_And_Ironfist,
            Gargant_Hacker
        };

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static void Init();

        MournfangPack();

        ~MournfangPack() override = default;

        bool configure(int numModels, WeaponOption weaponOption, bool hornblower, bool bannerBearer);

    protected:

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        Wounds computeReturnedDamage(const Weapon *weapon, int saveRoll) const override;

        void onWounded() override;

        void onRestore() override;

        int chargeModifier() const override;

        int braveryModifier() const override;

    private:

        WeaponOption m_option = Culling_Club_Or_Prey_Hacker_And_Ironfist;
        bool m_hornblower = false;
        bool m_bannerBearer = false;

        Weapon m_pistol,
                m_clubOrHacker,
                m_gargantHacker,
                m_tusks;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Hornblower                       Yes
// Banner Bearer                    Yes
// Ironfist                         Yes
// Mournfang Charge                 Yes
//

} // namespace OgorMawtribes

#endif //MOURNFANGPACK_H
