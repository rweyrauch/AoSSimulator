/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019-20 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef TZAANGORS_H
#define TZAANGORS_H

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

        static int ComputePoints(int numModels);

        static void Init();

        Tzaangors();

        ~Tzaangors() override = default;

        bool configure(int numModels, WeaponOptions weapons, int numGreatblades, int numMutants, bool iconBearer,
                       bool brayhorns);

    protected:

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnit) override;

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        int toWoundModifier(const Weapon *weapon, const Unit *target) const override;

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

    private:

        WeaponOptions m_weaponOption = Paired_Savage_Blades;
        int m_numGreatblades = 0;
        int m_numMutants = 0;

        Weapon m_savageBlade,
                m_savageBladeTwistbray,
                m_savageGreatblade,
                m_savageGreatbladeTwistbray,
                m_viciousBeak,
                m_viciousBeakTwistbray;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Ornate Totems                    TODO
// Brayhorns                        Yes
// Arcanite Shield                  Yes
// Destined Mayhem                  Yes
// Paired Savage Blades             Yes
// Savagery Unleashed               Yes
//

} // Tzeentch

#endif //TZAANGORS_H