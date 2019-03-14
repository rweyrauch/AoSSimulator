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
    static const int POINTS_PER_BLOCK = 200;
    static const int POINTS_MAX_UNIT_SIZE = 800;

    static Unit *Create(const ParameterList &parameters);

    static void Init();

    Evocators();

    ~Evocators() override = default;

    bool configure(int numModels, int numGrandstaves, bool primeGrandstave);

    void visitWeapons(std::function<void(const Weapon *)> &visitor) override;

protected:

    Rerolls toSaveRerolls(const Weapon *weapon) const override;

    int generateMortalWounds(const Unit *unit) override;

private:

    Weapon m_tempestBladeAndStave,
        m_tempestBladeAndStavePrime,
        m_grandStave,
        m_grandStavePrime;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Celestial Lightning Arc          Yes
// Empower                          Yes
//

} // namespace StormcastEternals

#endif //EVOCATORS_H