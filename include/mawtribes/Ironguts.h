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

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    Ironguts();
    ~Ironguts() override;

    bool configure(int numModels, bool runeMawBearer, bool bellower);

protected:

    int bellower(const Unit* target);

private:

    bool m_runeMawBearer = false;
    bool m_bellower = false;

    Weapon m_bashingWeapon,
        m_bite,
        m_bashingWeaponGutlord;

    lsignal::slot m_connection;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Bellower                         Yes
// Rune Maw Bearer                  TODO
// Down to the Ironguts             TODO
//

} // namespace OgorMawtribes

#endif //ironguts_h
