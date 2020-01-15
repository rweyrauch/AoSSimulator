/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef COS_HAMMERERS_H
#define COS_HAMMERERS_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar
{

class Hammerers : public CitizenOfSigmar
{
public:

    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 140;
    static const int POINTS_MAX_UNIT_SIZE = 360;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    Hammerers();
    ~Hammerers() override = default;

    bool configure(int numModels, bool standardBearer, bool musician);

protected:

    bool battleshockRequired() const override;
    int runModifier() const override;
    int chargeModifier() const override;
    int braveryModifier() const override;

private:

    bool m_standardBearer = false;
    bool m_musician = false;

    Weapon m_greatHammer,
        m_greatHammerKeeper;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Kingsguard                       Yes
// Musician                         Yes
// Standard Bearer                  Yes
// Musician                         Yes
//

} // namespace CitiesOfSigmar

#endif //COS_HAMMERERS_H