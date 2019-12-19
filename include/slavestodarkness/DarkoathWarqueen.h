/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef DARKOATHWARQUEEN_H
#define DARKOATHWARQUEEN_H

#include <Unit.h>
#include <Weapon.h>

namespace SlavesToDarkness
{

class DarkoathWarqueen : public Unit
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 90;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    DarkoathWarqueen();
    ~DarkoathWarqueen() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

private:

    Weapon m_axe;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Infernal Runeshield              No
// Savage Duellist                  No
// The Will of the Gods             No
//

} // SlavesToDarkness

#endif //DARKOATHWARQUEEN_H