/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef IRONGUTS_H
#define IRONGUTS_H

#include <mawtribes/MawtribesBase.h>
#include <Weapon.h>

namespace OgorMawtribes
{

class Ironguts : public MawtribesBase
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 4;
    static const int MIN_UNIT_SIZE = 4;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 220;
    static const int POINTS_MAX_UNIT_SIZE = 660;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    Ironguts();
    ~Ironguts() override = default;

    bool configure(int numModels, bool runeMawBearer, bool bellower);

protected:

private:

    bool m_runeMawBearer = false;
    bool m_bellower = false;

    Weapon m_bashingWeapon,
        m_bite,
        m_bashingWeaponGutlord;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Bellower                         No
// Rune Maw Bearer                  No
// Down to the Ironguts             No
//

} // namespace OgorMawtribes

#endif //ironguts_h
