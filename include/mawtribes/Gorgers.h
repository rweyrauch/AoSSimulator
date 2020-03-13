/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef GORGERS_H
#define GORGERS_H

#include <mawtribes/MawtribesBase.h>
#include <Weapon.h>

namespace OgorMawtribes
{

class Gorgers : public MawtribesBase
{
public:

    static const int BASESIZE = 50;
    static const int WOUNDS = 5;
    static const int MIN_UNIT_SIZE = 1;
    static const int MAX_UNIT_SIZE = 3;
    static const int POINTS_PER_BLOCK = 80;
    static const int POINTS_MAX_UNIT_SIZE = 240;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    Gorgers();
    ~Gorgers() override = default;

    bool configure(int numModels);

protected:

    // Instatiable Hunger
    Rerolls chargeRerolls() const override { return RerollFailed; }

private:

    Weapon m_claws,
        m_jaw;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Ambushing Hunters                TODO
// Insatiable Hunger                Yes

} // namespace OgorMawtribes

#endif //GORGERS_H
