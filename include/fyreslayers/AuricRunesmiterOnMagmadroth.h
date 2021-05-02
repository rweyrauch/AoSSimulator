/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <fyreslayers/Magmadroth.h>
#include <Weapon.h>

namespace Fyreslayers {

    class AuricRunesmiterOnMagmadroth : public Magmadroth {
    public:

        enum WeaponOption {
            Runic_Iron,
            Forge_Key
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        AuricRunesmiterOnMagmadroth();

        ~AuricRunesmiterOnMagmadroth() override = default;

    protected:

        void configure(Blessing blessing, WeaponOption weapons, MountTrait trait);

    private:

        WeaponOption m_weaponOption = Runic_Iron;

        Weapon m_throwingAxe,
                m_latchAxe,
                m_runicIron;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Roaring Fyrestream               Yes
// Lashing Tail                     Yes
// Volcanic Blood                   Yes
// Grand Ritual of Awakening        TODO
// Runic Empowerment                Yes
//

} // namespace Fyreslayers
