/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <idonethdeepkin/IdonethDeepkin.h>
#include <Weapon.h>

namespace IdonethDeepkin {

    class AkhelianKing : public IdonethDeepkinBase {
    public:

        enum WeaponOption {
            Bladed_Polearm,
            Greatsword,
        };

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        AkhelianKing();

        ~AkhelianKing() override = default;

    protected:

        void configure(WeaponOption weapon, MountTrait trait);

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onCharged() override;

    private:

        Weapon m_bladedPolearm,
                m_greatsword,
                m_falchion,
                m_deepmareJawsTalons,
                m_deepmareTails;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Deepmare Horn                    Yes
// Akhelian Paragon                 TODO
// Storm of Blows                   TODO
// Wave Rider                       Yes
// Lord of Tides                    TODO
//

} // namespace IdonethDeepkin

