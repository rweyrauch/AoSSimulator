/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019-20 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <tzeentch/TzeentchBase.h>
#include <Weapon.h>

namespace Tzeentch {

    class Tzaangors : public TzeentchBase {
    public:

        enum WeaponOptions {
            Paired_Savage_Blades,
            Savage_Blade_And_Shield
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Tzaangors(ChangeCoven coven, int numModels, WeaponOptions weapons, int numGreatblades, int numMutants, bool iconBearer,
                  bool brayhorns, int points);

        ~Tzaangors() override = default;

    protected:

        Wounds applyWoundSave(const Wounds &wounds, Unit *attackingUnit) override;

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        int toWoundModifier(const Weapon *weapon, const Unit *target) const override;

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        void onStartHero(PlayerId player) override;

    private:

        WeaponOptions m_weaponOption = Paired_Savage_Blades;
        int m_numGreatblades = 0;
        int m_numMutants = 0;

        Weapon  m_savageBlade{Weapon::Type::Melee, "Savage Blade", 1, 2, 4, 4, 0, 1},
                m_savageBladeTwistbray{Weapon::Type::Melee, "Savage Blade", 1, 2, 3, 4, 0, 1},
                m_savageGreatblade{Weapon::Type::Melee, "Savage Greatblade", 1, 1, 4, 4, -1, 2},
                m_savageGreatbladeTwistbray{Weapon::Type::Melee, "Savage Greatblade", 1, 1, 3, 4, -1, 2},
                m_viciousBeak{Weapon::Type::Melee, "Vicious Beak", 1, 1, 4, 5, 0, 1},
                m_viciousBeakTwistbray{Weapon::Type::Melee, "Vicious Beak", 1, 1, 3, 5, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Ornate Totems                    Yes
// Brayhorns                        Yes
// Arcanite Shield                  Yes
// Destined Mayhem                  Yes
// Paired Savage Blades             Yes
// Savagery Unleashed               Yes
//

} // Tzeentch
