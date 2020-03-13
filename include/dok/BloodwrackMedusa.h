/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BLOODWRACK_H
#define BLOODWRACK_H

#include <dok/DaughterOfKhaine.h>
#include <Weapon.h>

namespace DaughtersOfKhaine
{

class BloodwrackMedusa : public DaughterOfKhaine
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 140;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    BloodwrackMedusa();
    ~BloodwrackMedusa() override = default;

    bool configure();

protected:


private:

    Weapon m_bloodwrackStare,
        m_whisperclaw,
        m_tailOfSerpents,
        m_bloodwrackSpear;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Bloodwrack Stare                 TODO
// Enfeebling Foe                   TODO
//

} // namespace DaughtersOfKhaine

#endif //BLOODWRACK_H