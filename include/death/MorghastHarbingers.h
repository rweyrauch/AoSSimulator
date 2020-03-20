/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef MORGHASTHARBINGERS_H
#define MORGHASTHARBINGERS_H

#include <death/LegionOfNagash.h>
#include <Weapon.h>

namespace Death
{

class MorghastHarbingers : public LegionOfNagashBase
{
public:

    enum WeaponOptions
    {
        SpiritHalberd,
        SpiritSwords,
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static int ComputePoints(int numModels);
    static void Init();

    MorghastHarbingers();
    ~MorghastHarbingers() override = default;

    bool configure(int numModels, WeaponOptions weapons);

protected:

private:

    WeaponOptions m_weaponOption = SpiritHalberd;

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

} //namespace Death

#endif //MORGHASTHARBINGERS_H
