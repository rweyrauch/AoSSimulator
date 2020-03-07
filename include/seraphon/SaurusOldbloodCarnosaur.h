/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SAURUSOLDBLOODCARNOSAUR_H
#define SAURUSOLDBLOODCARNOSAUR_H

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon
{

class SaurusOldbloodOnCarnosaur : public SeraphonBase
{
public:

    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 250;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; }
    static void Init();

    SaurusOldbloodOnCarnosaur();
    ~SaurusOldbloodOnCarnosaur() override = default;

    bool configure();

    int move() const override;

protected:

    void onWounded() override;
    int getDamageTableIndex() const;

private:

    Weapon m_gauntlet,
        m_spear,
        m_forelimbs,
        m_jaws;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Blazing Sunbolts                 No
// Blood Frenzy                     No
// Cold Ferocity                    No
// Pinned Down                      No
// Terror                           No
// Wrath of the Seraphon            No
//

} // namespace Seraphon

#endif //SAURUSOLDBLOODCARNOSAUR_H