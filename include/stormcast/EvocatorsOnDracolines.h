/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef EVOCATORSONDRACOLINES_H
#define EVOCATORSONDRACOLINES_H

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals
{

class EvocatorsOnCelestialDracolines : public StormcastEternal
{
public:

    static const int BASESIZE = 90; // x52 oval
    static const int WOUNDS = 5;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 300;
    static const int POINTS_MAX_UNIT_SIZE = 1200;

    static Unit *Create(const ParameterList &parameters);

    static void Init();

    EvocatorsOnCelestialDracolines();

    ~EvocatorsOnCelestialDracolines() override = default;

    bool configure(int numModels, int numGrandstaves, bool primeGrandstave);

    void visitWeapons(std::function<void(const Weapon *)> &visitor) override;

protected:

    Rerolls toSaveRerolls(const Weapon *weapon) const override;

    int generateMortalWounds(const Unit *unit) override;

    Rerolls chargeRerolls() const override;

private:

    Weapon m_tempestBladeAndStave,
        m_tempestBladeAndStavePrime,
        m_grandStave,
        m_grandStavePrime,
        m_monstrousClaws;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Celestial Lightning Arc          Yes
// Supernatural Roar                No
// Thunderous Pounce                Yes
// Empower                          No
//

} // namespace StormcastEternals

#endif //EVOCATORSONDRACOLINES_H