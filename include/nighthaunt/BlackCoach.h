/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BLACKCOACH_H
#define BLACKCOACH_H

#include <nighthaunt/Nighthaunt.h>
#include <Weapon.h>

namespace Nighthaunt
{

class BlackCoach : public Nighthaunt
{
public:

    static const int BASESIZE = 170; // x105 oval
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 220;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; }
    static void Init();

    BlackCoach();
    ~BlackCoach() override = default;

    int move() const override;

    bool configure();

protected:

    void onWounded() override;
    int getDamageTableIndex() const;

private:

    Weapon m_graspMissile,
        m_scythe,
        m_grasp,
        m_claws,
        m_hoovesAndTeeth;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Ethereal                         Yes
// Frightful Touch                  No
// Evocation of Death               No
//     Nimbus of Power              No
//     Unholy Vigour                No
//     Spectral Scythes             No
//     Insubstantial Form           No
//     Witch-fire                   No
// Reaped Like Corn                 No
//


} // namespace Nighthaunt

#endif // BLACKCOACH_H
