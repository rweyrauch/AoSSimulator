/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef COS_WILDWOODRANGERS_H
#define COS_WILDWOODRANGERS_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar
{

class WildwoodRangers : public CitizenOfSigmar
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 130;
    static const int POINTS_MAX_UNIT_SIZE = 390;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    WildwoodRangers();
    ~WildwoodRangers() override = default;

    bool configure(int numModels, bool standardBearer, bool hornblower);

protected:

    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    int runModifier() const override;
    int chargeModifier() const override;
    int braveryModifier() const override;

private:

    bool m_hornblower = false,
        m_standardBearer = false;

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

#endif //COS_WILDWOODRANGERS_H