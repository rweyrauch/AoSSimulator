/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef RETRIBUTORS_H
#define RETRIBUTORS_H

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals
{

class Retributors : public StormcastEternal
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 3;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;

    Retributors();
    ~Retributors() override = default;

    bool configure(int numModels, int numStarsoulMaces);

protected:

private:

    static Weapon s_lightningHammer,
                  s_lightningHammerPrime,
                  s_starsoulMace;
};

} // namespace StormcastEternals

#endif //RETRIBUTORS_H