/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef FATEWEAVER_H
#define FATEWEAVER_H

#include <Unit.h>
#include <Weapon.h>

namespace Tzeentch
{

class KairosFateweaver : public Unit
{
public:

    static const int BASESIZE = 0;
    static const int WOUNDS = 14;
    static const int POINTS_PER_UNIT = 380;

    static Unit* Create(const ParameterList& parameters);

    static std::string ValueToString(const Parameter &parameter);

    static int EnumStringToInt(const std::string &enumString);

    static void Init();

    KairosFateweaver();
    ~KairosFateweaver() override = default;

    bool configure();
    int move() const override;

    int rollCasting() const override;

protected:

    void onWounded() override;
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
