/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef STORMVERMIN_H
#define STORMVERMIN_H

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven
{

class Stormvermin : public Skaventide
{
public:
    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 40;
    static const int POINTS_PER_BLOCK = 120;
    static const int POINTS_MAX_UNIT_SIZE = 450;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    Stormvermin();
    ~Stormvermin() override = default;

    bool configure(int numModels, bool clanshields, int standardBearers, int drummers);

protected:

    int toSaveModifier(const Weapon *weapon) const override;
    int runModifier() const override;

private:

    bool m_clanshields = false;
    int m_numStandardBearers = 0;
    int m_numDrummers = 0;

    Weapon m_rustyHalberd,
        m_rustyHalberdLeader;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Standard Bearer                  Yes
// Drummer                          Yes
// Clanshields                      Yes

} // namespace Skaven

#endif //STORMVERMIN_H