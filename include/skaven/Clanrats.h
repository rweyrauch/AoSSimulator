/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven {

    class Clanrats : public Skaventide {
    public:

        enum WeaponOptions {
            Rusty_Spear,
            Rusty_Blade
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        Clanrats();

        ~Clanrats() override = default;

        bool configure(int numModels, WeaponOptions weapons, bool clanshields, int standardBearers, int bellRingers);

    protected:

        int toSaveModifier(const Weapon *weapon, const Unit* attacker) const override;

        int runModifier() const override;

    private:

        bool m_clanshields = false;
        int m_numStandardBearers = 0;
        int m_numBellRingers = 0;

        Weapon m_rustySpear,
                m_rustySpearLeader,
                m_rustyBlade,
                m_rustyBladeLeader;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Standard Bearer                  Yes
// Bell-ringer                      Yes
// Clanshields                      Yes

} // namespace Skaven
