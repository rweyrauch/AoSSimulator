/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SLAUGHTERQUEEN_H
#define SLAUGHTERQUEEN_H

#include <dok/DaughterOfKhaine.h>
#include <Weapon.h>

namespace DaughtersOfKhaine
{

class SlaughterQueen : public DaughterOfKhaine
{
public:

    static const int BASESIZE = 25;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 100;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    SlaughterQueen();
    ~SlaughterQueen() override = default;

    bool configure();

protected:


private:

    Weapon m_bladeOfKhaine,
        m_deathsword;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Rune of Khaine                   No
// Wrath of Khaine                  No
// Touch of Death                   No
// Dance of Doom                    No
// Pact of Blood                    No
// Orgy of Slaughter                No
//

} // namespace DaughtersOfKhaine

#endif //SLAUGHTERQUEEN_H