/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef GREATUNCLEANONE_H
#define GREATUNCLEANONE_H

#include <nurgle/Nurgle.h>
#include <Weapon.h>

namespace Nurgle
{

class GreatUncleanOne : public NurgleBase
{
public:

    static const int BASESIZE = 130;
    static const int WOUNDS = 16;
    static const int POINTS_PER_UNIT = 0;

    enum WeaponOptionOne
    {
        PlagueFlail,
        Bileblade
    };

    enum WeaponOptionTwo
    {
        MassiveBilesword,
        DoomsdayBell
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    GreatUncleanOne();
    ~GreatUncleanOne() override = default;

    bool configure(WeaponOptionOne optionOne, WeaponOptionTwo optionTwo);

protected:

    void onWounded() override;
    void onCharged() override;
    int getDamageTableIndex() const;

private:

    Weapon m_bile,
        m_flail,
        m_bilesword,
        m_bileblade,
        m_doomsdayBell,
        m_nurglings;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Blubber and Bile                 No
// Corpulent Mass                   No
// Mountain of Loathsome Flesh      Yes
// Putrid Offerings                 No
// Reverberating Summons            No
// Plague Wind                      No
//

} // Nurgle

#endif //GREATUNCLEANONE_H