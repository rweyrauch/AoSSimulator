/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef DRYCHAHAMADRETH_H
#define DRYCHAHAMADRETH_H

#include <Unit.h>
#include <Weapon.h>

namespace Sylvaneth
{

class DrychaHamadreth : public Unit
{
public:

    static const int BASESIZE = 105; // x70 oval
    static const int WOUNDS = 10;
    static const int POINTS_PER_UNIT = 280;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    DrychaHamadreth();
    ~DrychaHamadreth() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    void onStartHero(PlayerId id) override;
    void onWounded() override;
    int getDamageTableIndex() const;

private:

    Weapon m_colonyOfFlitterfuries,
        m_swarmOfSquirmlings,
        m_slashingTalons,
        m_thornedSlendervines;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Colony of Flitterfuries          No
// Swarm of Squirmlings             No
// Mercurial Aspect                 No
// Song of Spite                    No
// Primal Terror                    No
//

} // namespace Sylvaneth

#endif //DRYCHAHAMADRETH_H