/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef GIANTRATS_H
#define GIANTRATS_H

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven
{

class GiantRats : public Skaventide
{
public:
    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 40;
    static const int POINTS_PER_BLOCK = 60;
    static const int POINTS_MAX_UNIT_SIZE = 200;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    GiantRats();
    ~GiantRats() override = default;

    bool configure(int numModels);

protected:

    void onWounded() override { setTeethRange(); }
    void onRestore() override { setTeethRange(); }

    void setTeethRange();

private:

    Weapon m_teeth;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Wave of Rats                     Yes
//

} // namespace Skaven

#endif //GIANTRATS_H