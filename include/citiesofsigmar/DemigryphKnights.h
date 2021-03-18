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

    class DemigryphKnights : public CitizenOfSigmar {
    public:

        enum WeaponOption {
            Halberd,
            Lance
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        DemigryphKnights();

        ~DemigryphKnights() override = default;

        bool configure(int numModels, bool standardBearer, bool hornblower, WeaponOption weapons);

    protected:

        int runModifier() const override;

        int chargeModifier() const override;

        int braveryModifier() const override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        Weapon m_halberd,
                m_lance,
                m_halberdPreceptor,
                m_lancePreceptor,
                m_beakAndTalons;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Standard Bearer                  Yes
// Hornblower                       Yes
// Charging Lance                   Yes
// Savage Ferocity                  Yes
//

} // namespace CitiesOfSigmar
