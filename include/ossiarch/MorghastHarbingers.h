/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <ossiarch/OssiarchBonereaperBase.h>
#include <Weapon.h>

namespace OssiarchBonereapers {

    class MorghastHarbingers : public OssiarchBonereaperBase {
    public:

        enum WeaponOptions {
            Spirit_Halberd,
            Spirit_Swords,
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        MorghastHarbingers();

        ~MorghastHarbingers() override = default;

        bool configure(int numModels, WeaponOptions weapons);

    protected:

    private:

        WeaponOptions m_weaponOption = Spirit_Halberd;

        Weapon m_spiritHalberd,
                m_spiritSwords;

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Heralds of the Accursed One      TODO
// Harbinger of Death               TODO
//

} //namespace OssiarchBonereapers

