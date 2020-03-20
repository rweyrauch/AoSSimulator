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
#include <spells/LoreOfTheStorm.h>
#include <Weapon.h>

namespace StormcastEternals {

    class Evocators : public StormcastEternal {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        Evocators();

        ~Evocators() override = default;

        bool configure(int numModels, int numGrandstaves, bool primeGrandstave, LoreOfInvigoration invigoration);

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
// Abilities                    Implemented
// -------------------------------------------
// Celestial Lightning Arc          Yes
// Empower                          Yes
//

} // namespace StormcastEternals

#endif //EVOCATORS_H