/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <dispossessed/Dispossessed.h>
#include <Weapon.h>

namespace Dispossessed {

    class Ironbreakers : public Dispossessed {
    public:

        enum WeaponOptions {
            Ironbreaker_Axe_Or_Hammer,
            Drakefire_Pistol_And_Cinderblast_Bomb,
            Paired_Drakefire_Pistols
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Ironbreakers(int points);

        ~Ironbreakers() override = default;

        bool configure(int numModels, WeaponOptions ironbeardWeapons, bool iconBearer, bool drummer);

    protected:

        Rerolls toSaveRerolls(const Weapon *weapon, const Unit* attacker) const override;

        int toSaveModifier(const Weapon *weapon, const Unit* attacker) const override;

        void onStartShooting(PlayerId player) override;

        int rollRunDistance() override;

    private:

        bool m_hasCinderblastBomb = false;

        Weapon m_drakefirePistol,
                m_drakefirePistolMelee,
                m_axeOrHammer,
                m_axeOrHammerIronbeard;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Brace of Drakefire Pistols       Yes
// Cinderblast Bomb                 Yes
// Gromril Shields                  Yes
// Forge-proven Gromril Armour      Yes
// Musician                         Yes
// Icon Bearer                      TODO
//

} // namespace Dispossessed
