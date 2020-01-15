/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SEEKERCHARIOTS_H
#define SEEKERCHARIOTS_H

#include <slaanesh/SlaaneshBase.h>
#include <Weapon.h>

namespace Slaanesh
{

class SeekerChariots : public SlaaneshBase
{
public:

    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 6;
    static const int MIN_UNIT_SIZE = 1;
    static const int MAX_UNIT_SIZE = 3;
    static const int POINTS_PER_BLOCK = 120;
    static const int POINTS_MAX_UNIT_SIZE = 360;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    SeekerChariots();
    ~SeekerChariots() override = default;

    bool configure(int numModels);

protected:

    void onCharged() override;

private:

    Weapon m_flensingWhips,
        m_piercingClaws,
        m_poisonedTongues;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Impossibly Swift                 Yes
// Multilating Blades               Yes

} // Slannesh

#endif //SEEKERCHARIOTS_H