/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef KNIGHTINCANTOR_H
#define KNIGHTINCANTOR_H

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals
{

class KnightIncantor : public StormcastEternal
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 140;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    KnightIncantor();
    ~KnightIncantor() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

private:

    static Weapon s_staff;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Voidstorm Scroll                 No
// Spirit Flask                     No
// Spirit Storm                     No
//

} // namespace StormcastEternals

#endif //KNIGHTINCANTOR_H