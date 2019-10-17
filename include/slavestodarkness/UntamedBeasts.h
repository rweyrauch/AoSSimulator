/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef UNTAMEDBEASTS_H
#define UNTAMEDBEASTS_H

#include <Unit.h>
#include <Weapon.h>

namespace SlavesToDarkness
{

class UntamedBeasts : public Unit
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 9;
    static const int MAX_UNIT_SIZE = 36;
    static const int POINTS_PER_BLOCK = 70;
    static const int POINTS_MAX_UNIT_SIZE = 280;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    UntamedBeasts();
    ~UntamedBeasts() override = default;

    bool configure(int numModels);

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

private:

    Weapon m_harpoonFirstFang,
        m_huntingWeapons,
        m_huntingWeaponsHeartEater;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Unleash the Beast                No
//

} // SlavesToDarkness

#endif //UNTAMEDBEASTS_H