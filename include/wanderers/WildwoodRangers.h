/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef WILDWOODRANGERS_H
#define WILDWOODRANGERS_H

#include <wanderers/Wanderer.h>
#include <Weapon.h>

namespace Wanderers
{

class WildwoodRangers : public Wanderer
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 140;
    static const int POINTS_MAX_UNIT_SIZE = 420;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    WildwoodRangers();
    ~WildwoodRangers() override = default;

    bool configure(int numModels, bool standardBearer, bool hornblower);

protected:

    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    Rerolls runRerolls() const override;
    int braveryModifier() const override;

private:

    bool m_hornblower,
        m_standardBearer;

    Weapon m_rangersDraich,
        m_wardensDraich;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Guardians of the Kindreds        Yes
// Standard Bearer                  Yes
// Hornblower                       Yes
//

} // namespace Wanderers

#endif //WILDWOODRANGERS_H