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

    class FreeguildGuard : public CitizenOfSigmar {
    public:

        enum WeaponOption {
            Halberd,
            Spear,
            Sword
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        FreeguildGuard(int points);

        ~FreeguildGuard() override = default;

        bool configure(int numModels, bool standardBearer, bool drummer, WeaponOption weapons);

    protected:

        int runModifier() const override;

        int chargeModifier() const override;

        int braveryModifier() const override;

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        int toSaveModifier(const Weapon *weapon, const Unit* attacker) const override;

    private:

        Weapon m_halberd,
                m_spear,
                m_sword,
                m_halberdSergeant,
                m_spearSergeant,
                m_swordSergeant;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Drummer                          Yes
// Standard Bearer                  Yes
// Massed Ranks                     Yes
// Parry and Block                  Yes
// Wall of Spears                   Yes
//

} // namespace CitiesOfSigmar
