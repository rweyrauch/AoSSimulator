/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon {

    class TerradonRiders : public SeraphonBase {
    public:

        enum WeaponOption {
            Starstrike_Javelins,
            Sunleech_Bolas
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        TerradonRiders();

        ~TerradonRiders() override = default;

        bool configure(int numModels, WeaponOption option);

    protected:

    private:

        Weapon m_javelin,
                m_javelinLeader,
                m_bolas,
                m_bolasLeader,
                m_jaws;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Deadly Cargo                     TODO
//

} // namespace Seraphon
