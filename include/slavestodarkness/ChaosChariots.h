/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CHAOSCHARIOTS_H
#define CHAOSCHARIOTS_H

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness {

    class ChaosChariots : public SlavesToDarknessBase {
    public:

        enum WeaponOption {
            GreatBladeAndWhip,
            WarFlailAndWhip
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        ChaosChariots();

        ~ChaosChariots() override = default;

        bool configure(int numModels, WeaponOption weapons);

    protected:

        void onRestore() override;

        void onCharged() override;

    private:

        Weapon m_greatBlade,
                m_flail,
                m_whip,
                m_greatBladeExalted,
                m_flailExalted,
                m_whipExalted,
                m_hooves;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Mark of Chaos                    Yes
// Don't Spare the Lash             Yes
// Swift Death                      Yes
//

} // SlavesToDarkness

#endif //CHAOSCHARIOTS_H