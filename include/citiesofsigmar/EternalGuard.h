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

    class EternalGuard : public CitizenOfSigmar {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(const ParameterList& parameters);

        EternalGuard(City city, int numModels, bool standardBearer, bool hornblower, bool gladeShields, int points);

        ~EternalGuard() override = default;

    protected:

        int runModifier() const override;

        int chargeModifier() const override;

        int braveryModifier() const override;

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        int toWoundModifier(const Weapon *weapon, const Unit *target) const override;

        int toSaveModifier(const Weapon *weapon, const Unit* attacker) const override;

    private:

        bool m_gladeShields = false;

        Weapon m_spearStave,
                m_spearStaveWarden;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Fortress of Boughs               Yes
// Form Fortress of Boughs          Yes
// Standard Bearer                  Yes
// Hornblower                       Yes
//

} // namespace CitiesOfSigmar
