/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BLADEBRINGEREXALTEDCHARIOT_H
#define BLADEBRINGEREXALTEDCHARIOT_H

#include <slaanesh/SlaaneshBase.h>
#include <Weapon.h>

namespace Slaanesh
{

class BladebringerOnExaltedChariot : public SlaaneshBase
{
public:

    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 10;
    static const int POINTS_PER_UNIT = 220;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    BladebringerOnExaltedChariot();
    ~BladebringerOnExaltedChariot() override = default;

    bool configure();

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
// Excess of Blades                 Yes
// Pungent Soulscent                No
// Acquiescence                     No
//

} // Slannesh

#endif //BLADEBRINGEREXALTEDCHARIOT_H