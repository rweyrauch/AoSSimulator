/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef MORGHASTARCHAI_H
#define MORGHASTARCHAI_H

#include <ossiarch/OssiarchBonereaperBase.h>
#include <Weapon.h>

namespace OssiarchBonereapers {

    class MorghastArchai : public OssiarchBonereaperBase {
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

        MorghastArchai();

        ~MorghastArchai() override = default;

        bool configure(int numModels, WeaponOptions weapons);

    protected:

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnit) override;

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
// Ebon-wrought Armour              Yes
//

} //namespace OssiarchBonereapers

#endif //MORGHASTARCHAI_H
