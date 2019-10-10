/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#ifndef SAVAGEBIGSTABBAS_H
#define SAVAGEBIGSTABBAS_H

#include "Unit.h"

namespace Bonesplitterz
{

class SavageBigStabbas : public Unit
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 4;
    static const int MIN_UNIT_SIZE = 2;
    static const int MAX_UNIT_SIZE = 8;
    static const int POINTS_PER_BLOCK = 100;
    static const int POINTS_MAX_UNIT_SIZE = 400;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    SavageBigStabbas();
    ~SavageBigStabbas() override = default;

    bool configure(int numModels);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:


private:

    Weapon m_gorkToof;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Da Final Fling                   No
// The Bigger They Are              No
// Savagely Enthusiastic            No
//

} // namespace Bonesplitterz

#endif //SAVAGEBIGSTABBAS_H