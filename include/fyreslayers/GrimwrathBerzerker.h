/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef GRIMWRATHBERZERKER_H
#define GRIMWRATHBERZERKER_H

#include <fyreslayers/Fyreslayer.h>
#include <Weapon.h>

namespace Fyreslayers
{

class GrimwrathBerzerker : public Fyreslayer
{
public:

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    GrimwrathBerzerker();
    ~GrimwrathBerzerker() override = default;

    bool configure();

protected:

    Wounds applyWoundSave(const Wounds &wounds) override;

private:

    Weapon m_throwingAxe,
        m_greatAxe;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Unstoppable Berzerker            Yes
// Battle-fury                      TODO
// Dead, But Not Defeated           TODO
//

} // namespace Fyreslayers

#endif //GRIMWRATHBERZERKER_H