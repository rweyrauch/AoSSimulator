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

    class DarkRiders : public CitizenOfSigmar {
    public:

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        DarkRiders();

        ~DarkRiders() override;

        bool configure(int numModels, bool standardBearer, bool hornblower);

    protected:

        int chargeModifier() const override;

        int braveryModifier() const override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int sowTerrorAndConfusion(const Unit *target);

    private:

        Weapon m_crossbow,
                m_spear,
                m_bite,
                m_crossbowHerald;

        lsignal::slot m_connection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Standard Bearer                  Yes
// Hornblower                       Yes
// Murderous Charge                 Yes
// Sow Terror and Confusion         Yes
//

} // namespace CitiesOfSigmar
