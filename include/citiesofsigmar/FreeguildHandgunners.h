/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef FREEGUILDHANDGUNNERS_H
#define FREEGUILDHANDGUNNERS_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar {

    class FreeguildHandgunners : public CitizenOfSigmar {
    public:

        enum WeaponOption {
            Handgun,
            Long_Rifle,
            Repeater_Handgun
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        FreeguildHandgunners();

        ~FreeguildHandgunners() override = default;

        bool configure(int numModels, bool standardBearer, bool piper, WeaponOption marksmanWeapon);

    protected:

        int runModifier() const override;

        int chargeModifier() const override;

        int braveryModifier() const override;

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon m_freeguildHandgun,
                m_dagger,
                m_longRifle,
                m_repeaterHandgun,
                m_handgunMarksman;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Standard Bearer                  Yes
// Piper                            Yes
// Stand and Shoot                  TODO
// Steady Aim                       Yes
// Crack Shot                       TODO
//

} // namespace CitiesOfSigmar

#endif //FREEGUILDHANDGUNNERS_H