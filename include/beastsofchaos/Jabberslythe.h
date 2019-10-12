/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef JABBERSLYTHE_H
#define JABBERSLYTHE_H

#include <beastsofchaos/BeastsOfChaosBase.h>
#include <Weapon.h>

namespace BeastsOfChaos
{

class Jabberslythe : public BeastsOfChaosBase
{
public:

    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 10;
    static const int POINTS_PER_UNIT = 160;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    Jabberslythe();
    ~Jabberslythe() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:


private:

    Weapon m_slytheyTongue,
        m_vorpalClaws,
        m_spikedTail;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Aura of Madness                  No
// Spurting Bile Blood              No
//

} // namespace BeastsOfChaos

#endif //JABBERSLYTHE_H