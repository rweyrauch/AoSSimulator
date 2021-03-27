/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven {

    class Deathmaster : public Skaventide {
    public:

        enum WeaponOption {
            Weeping_Blades,
            Fighting_Claws
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        Deathmaster();

        ~Deathmaster() override = default;

    protected:

        void configure(WeaponOption option);

        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

    private:

        Weapon m_stars,
                m_blades,
                m_claws;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Hidden Killer                    TODO
// Running Death                    Yes
// Throwing Stars                   Yes
//

} // namespace Skaven
