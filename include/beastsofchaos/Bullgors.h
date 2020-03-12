/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BULLGORS_H
#define BULLGORS_H

#include <beastsofchaos/BeastsOfChaosBase.h>
#include <Weapon.h>

namespace BeastsOfChaos
{

class Bullgors : public BeastsOfChaosBase
{
public:

    static const int BASESIZE = 50;
    static const int WOUNDS = 4;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 140;
    static const int POINTS_MAX_UNIT_SIZE = 140*4;

    enum WeaponOptions
    {
        BullgorAxe,
        PairedBullgorAxes,
        BullgorGreatAxe
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static int ComputePoints(int numModels);
    static void Init();

    Bullgors();
    ~Bullgors() override = default;

    bool configure(int numModels, WeaponOptions options, bool drummer, bool bannerBearer);

protected:

    Rerolls toHitRerolls(const Weapon* weapon, const Unit* unit) const override;
    int chargeModifier() const override;
    int toSaveModifier(const Weapon* weapon) const override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

private:

    bool m_drummer = false;
    bool m_bannerBearer = false;
    bool m_pairedAxes = false;

    Weapon m_bullgorHorns;
    Weapon m_bullgorAxe;
    Weapon m_bullgorAxeBloodkine;
    Weapon m_bullgorGreatAxe;
    Weapon m_bullgorGreatAxeBloodkine;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Bloodgreed                       Yes
// Dual Axes                        Yes
// Bullshields                      Yes
//

} // namespace BeastsOfChaos

#endif //BULLGORS_H