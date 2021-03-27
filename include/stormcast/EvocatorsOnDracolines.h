/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <stormcast/StormcastEternals.h>
#include <stormcast/LoreOfTheStorm.h>
#include <Weapon.h>

namespace StormcastEternals {

    class EvocatorsOnCelestialDracolines : public StormcastEternal {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        EvocatorsOnCelestialDracolines();

        ~EvocatorsOnCelestialDracolines() override;

        bool configure(int numModels, int numGrandstaves, bool primeGrandstave, Lore lore);

    protected:

        Rerolls toSaveRerolls(const Weapon *weapon, const Unit *attacker) const override;

        int generateMortalWounds(const Unit *unit) override;

        Rerolls chargeRerolls() const override;

        int supernaturalRoar(const Unit *target);

    private:

        Weapon m_tempestBladeAndStave,
                m_tempestBladeAndStavePrime,
                m_grandStave,
                m_grandStavePrime,
                m_monstrousClaws;

        lsignal::slot m_connection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Celestial Lightning Arc          Yes
// Supernatural Roar                Yes
// Thunderous Pounce                Yes
// Empower                          Yes
//

} // namespace StormcastEternals
