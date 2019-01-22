/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BLOODSISTERS_H
#define BLOODSISTERS_H

#include <dok/DaughterOfKhaine.h>
#include <Weapon.h>

namespace DaughtersOfKhaine
{

class BloodSisters : public DaughterOfKhaine
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    BloodSisters();
    ~BloodSisters() override = default;

    bool configure(int numModels);

protected:

    int generateMortalWounds(const Weapon* weapon, const Unit* unit, const Hits& hits) const override;

private:

    static Weapon s_heartshardGlaive,
        s_heartshardGlaiveGorgai,
        s_crystalTouch,
        s_crystalTouchGorgai;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Turned to Crystal                Yes
//

} // namespace DaughtersOfKhaine

#endif //BLOODSISTERS_H