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

    class Cogsmith : public CitizenOfSigmar {
    public:

        enum WeaponOption {
            Grudge_Raker,
            Cog_Axe
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int ComputePoints(const ParameterList& parameters);

        static int EnumStringToInt(const std::string &enumString);

        static void Init();

        Cogsmith(City city, WeaponOption option, Narcotic narcotic, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~Cogsmith() override = default;

    protected:

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        void onStartHero(PlayerId player) override;

    private:

        WeaponOption m_weaponOption = Grudge_Raker;

        Weapon m_grudgeRaker,
                m_pistols,
                m_cogAxe,
                m_gunButt;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Free Arm                         Yes
// Master Engineer                  Yes
//

} // namespace CitiesOfSigmar
