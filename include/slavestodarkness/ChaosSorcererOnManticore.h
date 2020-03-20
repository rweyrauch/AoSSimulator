/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SORCERERONMANTICORE_H
#define SORCERERONMANTICORE_H

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness
{

class ChaosSorcererOnManticore : public SlavesToDarknessBase
{
public:

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    ChaosSorcererOnManticore();
    ~ChaosSorcererOnManticore() override = default;

    bool configure();

protected:

    void onWounded() override;
    void onRestore() override;
    int getDamageTableIndex() const;

    Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

private:

    Weapon m_staff,
        m_fangsAndClaws,
        m_tail;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Oracular Visions                 TODO
// Wind of Chaos                    TODO
// Territorial Predator             Yes
// Mark of Chaos                    Yes
//

} // SlavesToDarkness

#endif //SORCERERONMANTICORE_H