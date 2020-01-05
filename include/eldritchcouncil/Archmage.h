/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ARCHMAGE_H
#define ARCHMAGE_H

#include <Unit.h>
#include <Weapon.h>

namespace EldritchCouncil
{

class Archmage : public Unit
{
public:
    static const int BASESIZE = 25;
    static const int BASESIZE_STEED = 60; // x35 oval
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 100;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    Archmage();
    ~Archmage() override = default;

    bool configure(bool steed);

protected:

private:

    bool m_steed = false;

    Weapon m_seerstaff,
        m_steedHooves;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Talisman of Arcane Power         No
// Elemental Shield                 No
//

} // namespace EldritchCouncil

#endif //ARCHMAGE_H