/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SKRYEACOLYTES_H
#define SKRYEACOLYTES_H

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven
{

class SkryeAcolytes : public Skaventide
{
public:
    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 60;
    static const int POINTS_MAX_UNIT_SIZE = 320;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    SkryeAcolytes();
    ~SkryeAcolytes() override = default;

    bool configure(int numModels);

protected:

    int toHitModifier(const Weapon *weapon, const Unit *target) const override;

private:

    Weapon m_globe,
        m_knife;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Quick-quick Volley!              Yes
// Gas Clouds                       Yes
//

} // namespace Skaven

#endif //SKRYEACOLYTES_H