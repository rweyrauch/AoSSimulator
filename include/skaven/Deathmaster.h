/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef DEATHMASTER_H
#define DEATHMASTER_H

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

        bool configure(WeaponOption option);

    protected:

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

#endif //DEATHMASTER_H