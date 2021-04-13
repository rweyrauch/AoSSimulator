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

    class ScourgerunnerChariots : public CitizenOfSigmar {
    public:

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        ScourgerunnerChariots();

        ~ScourgerunnerChariots() override = default;

        bool configure(int numModels);

    protected:

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        Weapon m_harpoon,
                m_crossbow,
                m_hookSpear,
                m_bite,
                m_harpoonMaster,
                m_crossbowMaster;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Lay the Beast Low                Yes
//

} // namespace CitiesOfSigmar
