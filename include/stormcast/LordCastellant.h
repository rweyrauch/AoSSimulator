/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LORDCASTELLANT_H
#define LORDCASTELLANT_H

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals
{

class LordCastellant : public StormcastEternal
{
public:

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    LordCastellant();
    ~LordCastellant() override = default;

    bool configure();

protected:

private:

    Weapon m_halberd;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Faithful Gryph-hound             TODO
// Warding Lantern                  TODO
//

} // namespace StormcastEternals

#endif //LORDCASTELLANT_H