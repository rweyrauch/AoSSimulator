/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef FATEWEAVER_H
#define FATEWEAVER_H

#include <tzeentch/TzeentchBase.h>
#include <Weapon.h>

namespace Tzeentch
{

class KairosFateweaver : public TzeentchBase
{
public:

    static const int BASESIZE = 100;
    static const int WOUNDS = 14;
    static const int POINTS_PER_UNIT = 400;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    KairosFateweaver();
    ~KairosFateweaver() override = default;

    bool configure();

    int rollCasting() const override;

protected:

    void onWounded() override;
    void onRestore() override;
    int getDamageTableIndex() const;

private:

    Weapon m_staff,
        m_beakAndTalons;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Master of Magic                  Yes
// Spell-eater                      No
// Oracle of Eternity               No
// Gift of Change                   No
//

} // namespace Tzeentch

#endif //FATEWEAVER_H
