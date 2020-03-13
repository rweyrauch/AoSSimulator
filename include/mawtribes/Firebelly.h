/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef FIREBELLY_H
#define FIREBELLY_H

#include <mawtribes/MawtribesBase.h>
#include <Weapon.h>

namespace OgorMawtribes
{

class Firebelly : public MawtribesBase
{
public:

    static const int BASESIZE = 50;
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 120;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    Firebelly();
    ~Firebelly() override = default;

    bool configure();

protected:

    void onStartShooting(PlayerId player) override;

private:

    Weapon m_fireBreath,
        m_hammer;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Fire Breath                      Yes
// Cascading Fire-cloak             TODO
//

} // namespace OgorMawtribes

#endif //FIREBELLY_H