/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef TZAANGORS_H
#define TZAANGORS_H

#include <Unit.h>
#include <Weapon.h>

namespace Tzeentch
{

class Tzaangors : public Unit
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 180;
    static const int POINTS_MAX_UNIT_SIZE = 480;

    enum WeaponOptions
    {
        PairedSavageBlades,
        SavageBladeAndShield
    };

    static Unit* Create(const ParameterList& parameters);

    static std::string ValueToString(const Parameter &parameter);

    static int EnumStringToInt(const std::string &enumString);

    static void Init();

    Tzaangors();
    ~Tzaangors() override = default;

    bool configure(int numModels, WeaponOptions weapons, int numGreatblades, int numMutants, bool iconBearer, bool brayhorns);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    Wounds applyWoundSave(const Wounds &wounds) override;
    int toHitModifier(const Weapon *weapon, const Unit *target) const override;
    int toWoundModifier(const Weapon *weapon, const Unit *target) const override;
    int extraAttacks(const Weapon *weapon) const override;

private:

    WeaponOptions m_weapons = PairedSavageBlades;
    int m_numGreatblades = 0;
    int m_numMutants = 0;
    bool m_iconBearer = false;
    bool m_brayhorns = false;

    Weapon m_savageBlade,
        m_savageBladeTwistbray,
        m_savageGreatblade,
        m_savageGreatbladeTwistbray,
        m_viciousBeak,
        m_viciousBeakTwistbray;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Icon Bearers                     No
// Brayhorns                        Yes
// Arcanite Shield                  Yes
// Anarchy and Mayhem               Yes
// Paired Savage Blades             Yes
// Savagery Unleashed               Yes
//

} // Tzeentch

#endif //TZAANGORS_H