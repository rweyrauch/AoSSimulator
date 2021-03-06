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

    class Skywardens : public KharadronBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        Skywardens();

        ~Skywardens() override = default;

        bool configure(int numModels, int numVolleyGuns, int numDrills, int numGrapnels, int numSkyhooks);

    protected:

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int chargeModifier() const override;

    private:

        Weapon m_volleyGun,
                m_skyhook,
                m_drillLauncher,
                m_pistol,
                m_gunButt,
                m_skypike,
                m_gunButtCustodian,
                m_skypikeCustodian;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Drill Launcher                   Yes
// Grapnel Launcher                 TODO
// Hitchers                         TODO
// Skyhook                          Yes
// Skymines                         TODO
// Timed Charges                    TODO
//

} // namespace KharadronOverlords

