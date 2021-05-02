/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <fyreslayers/Fyreslayer.h>
#include <Weapon.h>

namespace Fyreslayers {

    class AuricRunesmiter : public Fyreslayer {
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

        AuricRunesmiter();

        ~AuricRunesmiter() override = default;

    protected:

        void configure(Blessing blessing, WeaponOption weapons);

    private:

        WeaponOption m_weaponOption = Runic_Iron;

        Weapon m_throwingAxe,
                m_runicIron,
                m_latchAxe;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Magmic Tunnelling                TODO
// Runic Empowerment                Yes
//

} // namespace Fyreslayers
