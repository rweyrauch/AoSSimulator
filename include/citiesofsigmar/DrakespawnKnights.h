/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar {

    class DrakespawnKnights : public CitizenOfSigmar {
    public:

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        DrakespawnKnights(City city, int numModels, bool standardBearer, bool hornblower, int points);

        ~DrakespawnKnights() override = default;

    protected:

        int runModifier() const override;

        int chargeModifier() const override;

        int braveryModifier() const override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int weaponRend(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        Weapon m_lance,
                m_lanceDreadKnight,
                m_jaws;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Standard Bearer                  Yes
// Hornblower                       Yes
// Lance Charge                     Yes
//

} // namespace CitiesOfSigmar
