/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef VARGHULFCOURTIER_H
#define VARGHULFCOURTIER_H

#include <fec/FleshEaterCourts.h>
#include <Weapon.h>

namespace FleshEaterCourt
{

class VarghulfCourtier : public FleshEaterCourts
{
public:
    static const int BASESIZE = 60;
    static const int WOUNDS = 8;
    static const int POINTS_PER_UNIT = 160;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    VarghulfCourtier();
    ~VarghulfCourtier() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

private:

    Weapon m_immenseClaws,
        m_daggerlikeFangs;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Feed on Dark Magic               No
// King;s Champion                  No
// Victory Feast                    No
// Muster Royal Household           No
//

} // namespace FleshEaterCourt

#endif //VARGHULFCOURTIER_H