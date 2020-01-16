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
#include <Weapon.h>

namespace OssiarchBonereapers
{

class OssiarchBonereaperBase : public Unit
{
public:

    enum Legion
    {
        None,
        MortisPraetorians,
        PetrifexElite,
        StalliarchLords,
        IvoryHost,
        NullMyriad,
        Crematorians,
    };

    OssiarchBonereaperBase() = default;
    ~OssiarchBonereaperBase() override = default;

    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);

    void setLegion(Legion legion);

protected:
    OssiarchBonereaperBase(const std::string& name, int move, int wounds, int bravery, int save, bool fly) :
        Unit(name, move, wounds, bravery, save, fly) {}

    Wounds applyWoundSave(const Wounds& wounds) override;

protected:

    Legion m_legion = None;

};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Deathless Warriors               Yes
// Ranks Unbroken by Dissent        No
//

void Init();

} // namespace OssiarchBonereapers

#endif // OSSIARCHBASE_H