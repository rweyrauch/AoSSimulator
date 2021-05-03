/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <kharadron/KharadronBase.h>
#include <Weapon.h>

namespace KharadronOverlords {

    class ArkanautCompany : public KharadronBase {
    public:

        enum CaptainWeapon {
            Aetherflare_Pistol,
            Volley_Pistol,
            Privateer_Pistol
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        ArkanautCompany(Skyport port, Artycle artycle, Amendment amendment, Footnote footnote,
                        int numModels, int numVolleyGuns, int numSkyhooks, int numSkypikes, CaptainWeapon option, int points);

        ~ArkanautCompany() override = default;

    protected:

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        Rerolls battleshockRerolls() const override;

    private:

        Weapon m_privateerPistol,
                m_volleyGun,
                m_skyhook,
                m_aetherflarePistol,
                m_volleyPistol,
                m_cutter,
                m_gunButt,
                m_skypike;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Glory-seekers                    Yes
//

} // namespace KharadronOverlords

