/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef HAGQUEEN_H
#define HAGQUEEN_H

#include <dok/DaughterOfKhaine.h>
#include <Weapon.h>

namespace DaughtersOfKhaine
{

class HagQueen : public DaughterOfKhaine
{
public:

    static const int BASESIZE = 25;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 90;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    HagQueen();
    ~HagQueen() override = default;

    bool configure();

protected:


private:

    Weapon m_bladeOfKhaine;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Rune of Khaine                   TODO
// Wrath of Khaine                  TODO
// Touch of Death                   TODO
// Witchbrew                        TODO
//

} // namespace DaughtersOfKhaine

#endif //HAGQUEEN_H