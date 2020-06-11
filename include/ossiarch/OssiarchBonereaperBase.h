/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef OSSIARCHBASE_H
#define OSSIARCHBASE_H

#include <Unit.h>
#include <UnitFactory.h>
#include <Weapon.h>
#include <array>

namespace OssiarchBonereapers {

    enum Legion {
        NoLegion,
        MortisPraetorians,
        PetrifexElite,
        StalliarchLords,
        IvoryHost,
        NullMyriad,
        Crematorians,
    };

    const std::array<int, 7> g_legion = {
            NoLegion,
            MortisPraetorians,
            PetrifexElite,
            StalliarchLords,
            IvoryHost,
            NullMyriad,
            Crematorians,
    };

    class OssiarchBonereaperBase : public Unit {
    public:

        OssiarchBonereaperBase() = default;

        ~OssiarchBonereaperBase() override = default;

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        void setLegion(Legion legion);

    protected:
        OssiarchBonereaperBase(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

        Wounds applyWoundSave(const Wounds &wounds) override;

    protected:

        Legion m_legion = NoLegion;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Deathless Warriors               Yes
// Ranks Unbroken by Dissent        TODO
//

    void Init();

} // namespace OssiarchBonereapers

#endif // OSSIARCHBASE_H