/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SKINKS_H
#define SKINKS_H

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon {

    class Skinks : public SeraphonBase {
    public:

        enum WeaponOption {
            Javelins_Daggers_And_Bucklers,
            Boltspitters_And_Clubs,
            Boltspitters_Daggers_And_Bucklers,
            Clubs_And_Bucklers
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        Skinks();

        ~Skinks() override = default;

        bool configure(int numModels, WeaponOption weapons);

    protected:

        int toSaveModifier(const Weapon *weapon, const Unit* attacker) const override;

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

    private:

        WeaponOption m_weaponOption = Boltspitters_Daggers_And_Bucklers;

        Weapon m_javelin,
                m_boltspitter,
                m_dagger,
                m_daggerAlpha,
                m_club,
                m_clubAlpha;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Star-buckler                     Yes
// Swarming Cohort                  Yes
//

} // namespace Seraphon

#endif //SKINKS_H