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

    class Warriors : public Dispossessed {
    public:

        enum WeaponOptions {
            Duardin_Axe_Or_Hammer,
            Double_Handed_Duardin_Axe
        };

        enum StandardOptions {
            None,
            Runic_Icon,
            Clan_Banner
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        Warriors();

        ~Warriors() override = default;

        bool configure(int numModels, WeaponOptions weapons, bool duardinShields, StandardOptions standard,
                       bool hornblowers);

    protected:

        Rerolls toSaveRerolls(const Weapon *weapon, const Unit* attacker) const override;

        Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

        void onStartCombat(PlayerId player) override;

        int rollRunDistance() const override;

        void computeBattleshockEffect(int roll, int &numFled, int &numAdded) const override;

    private:

        bool m_duardinShields = false;

        bool m_opponentsCombat = false;

        Weapon m_duardinAxeOrHammer,
                m_duardinAxeOrHammerVeteran,
                m_doubleHandedAxe,
                m_doubleHandedAxeVeteran;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Resolute in Defence              Yes
// Duardin Shields                  Yes
// Runic Icon                       TODO
// Clan Banner                      Yes
// Hornblowers                      Yes
//

} // namespace Dispossessed
