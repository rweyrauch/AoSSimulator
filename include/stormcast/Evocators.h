/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef EVOCATORS_H
#define EVOCATORS_H

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals
{

class Evocators : public StormcastEternal
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 3;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;

    Evocators();
    ~Evocators() override = default;

    bool configure(int numModels, int numGrandstaves, bool primeGrandstave);

protected:

    Rerolls toSaveRerolls(const Weapon* weapon) const override;
    int generateMortalWounds(const Weapon* weapon, const Unit* unit, const Hits& hits) const override;

private:

    static Weapon s_tempestBladeAndStave,
        s_tempestBladeAndStavePrime,
        s_grandStave,
        s_grandStavePrime;
};

} // namespace StormcastEternals

#endif //EVOCATORS_H