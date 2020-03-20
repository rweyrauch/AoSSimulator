/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef FREEOUTRIDERS_H
#define FREEOUTRIDERS_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar {

    class FreeguildOutriders : public CitizenOfSigmar {
    public:

        enum WeaponOption {
            RepeaterHandgun,
            Blunderbuss,
            BraceOfPistols
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        FreeguildOutriders();

        ~FreeguildOutriders() override = default;

        bool configure(int numModels, bool trumpeter, WeaponOption sharpshooterWeapon);

    protected:

        int runModifier() const override;

        int chargeModifier() const override;

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

    private:

        bool m_trumpeter = false;

        Weapon m_blunderbuss,
                m_pistols,
                m_handgun,
                m_sabre,
                m_hooves,
                m_sabreSharpshooter;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Trumpeter                        Yes
// Expert Gunners                   Yes
// Skilled Riders                   Yes
//

} // namespace CitiesOfSigmar

#endif //FREEHANDGUNNER_H