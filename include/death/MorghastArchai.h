/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef MORGHASTARCHAI_H
#define MORGHASTARCHAI_H

#include <Unit.h>
#include <Weapon.h>

namespace Death
{

class MorghastArchai : public Unit
{
public:

    static const int BASESIZE = 60;
    static const int WOUNDS = 6;
    static const int MIN_UNIT_SIZE = 2;
    static const int MAX_UNIT_SIZE = 6;
    static const int POINTS_PER_BLOCK = 220;
    static const int POINTS_MAX_UNIT_SIZE = 660;

    enum WeaponOptions
    {
        SpiritHalberd,
        SpiritSwords,
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static void Init();

    MorghastArchai();
    ~MorghastArchai() override = default;

    bool configure(int numModels, WeaponOptions weapons);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

private:

    WeaponOptions m_weapons = SpiritHalberd;

    Weapon m_spiritHalberd,
        m_spiritSwords;

    static bool s_registered;

};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Heralds of the Accursed One      No
// Ebon-wrought Armour              No
//

} //namespace Death

#endif //MORGHASTARCHAI_H
