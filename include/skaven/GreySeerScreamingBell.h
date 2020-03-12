/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef GREYSEERONSCREAMINGBELL_H
#define GREYSEERONSCREAMINGBELL_H

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven
{

class GreySeerOnScreamingBell : public Skaventide
{
public:
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 13;
    static const int POINTS_PER_UNIT = 240;

    static Unit *Create(const ParameterList &parameters);

    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };

    static void Init();

    GreySeerOnScreamingBell();

    ~GreySeerOnScreamingBell() override = default;

    bool configure();

protected:

    Wounds applyWoundSave(const Wounds &wounds) override;
    void onWounded() override;
    void onRestore() override;

    int getDamageTableIndex() const;

private:

    Weapon m_staff,
        m_clawsAndFangs,
        m_spikes;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Altar of the Horned Rat
// Protection of the Horned Rat     Yes
// Pushed into Battle
// Peal of Doom
// Cracks Call
//

} // namespace Skaven

#endif //GREYSEERONSCREAMINGBELL_H