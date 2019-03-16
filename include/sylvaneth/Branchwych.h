/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BRANCHWYCH_H
#define BRANCHWYCH_H

#include <Unit.h>
#include <Weapon.h>

namespace Sylvaneth
{

class Branchwych : public Unit
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 240;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    Branchwych();
    ~Branchwych() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

private:

    Weapon m_greenwoodScythe,
        m_bittergrubsMandibles;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Fury of the Forest               No
// Quick Tempered                   No
// Unleash Spites                   No
//

} // namespace Sylvaneth

#endif //BRANCHWYCH_H