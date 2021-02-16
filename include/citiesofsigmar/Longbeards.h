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

    class Longbeards : public CitizenOfSigmar {
    public:

        enum WeaponOptions {
            Ancestral_Weapon_And_Shield,
            Ancestral_Great_Axe
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        Longbeards();

        ~Longbeards() override = default;

        bool configure(int numModels, WeaponOptions weapons, bool standardBearer, bool musician);

    protected:

        int runModifier() const override;

        int chargeModifier() const override;

        int braveryModifier() const override;

        Rerolls toSaveRerolls(const Weapon *weapon, const Unit* attacker) const override;

    private:

        WeaponOptions m_weaponOption = Ancestral_Weapon_And_Shield;

        Weapon m_ancestralAxeHammer,
                m_ancestralGreatAxe,
                m_ancestralAxeHammerOldGuard,
                m_ancestralGreatAxeOldGuard;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Gromril Shields                  Yes
// Old Grumblers                    TODO
// Standard Bearer                  Yes
// Musician                         Yes
//

} // namespace CitiesOfSigmar
