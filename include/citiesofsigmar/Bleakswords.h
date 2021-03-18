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

    class Bleakswords : public CitizenOfSigmar {
    public:

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        Bleakswords();

        ~Bleakswords() override = default;

        bool configure(int numModels, bool standardBearer, bool hornblower);

    protected:

        int runModifier() const override;

        int chargeModifier() const override;

        int braveryModifier() const override;

        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon m_sword, m_swordLordling;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Standard Bearer                  Yes
// Hornblower                       Yes
// Quicksilver Strike               Yes
// Ranks of Cold Steel              Yes
//

} // namespace CitiesOfSigmar
