/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BLOODREAVERS_H
#define BLOODREAVERS_H

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne
{

class Bloodreavers : public KhorneBase
{
public:
    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 40;
    static const int POINTS_PER_BLOCK = 70;
    static const int POINTS_MAX_UNIT_SIZE = 240;

    enum WeaponOption
    {
        ReaverBlades,
        MeatripperAxe
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static int ComputePoints(int numModels);
    static void Init();

    Bloodreavers();
    ~Bloodreavers() override = default;

    bool configure(int numModels, WeaponOption weapons, bool iconBearer, bool hornblowers);

protected:

    Rerolls toHitRerolls(const Weapon* weapon, const Unit* unit) const override;
    int braveryModifier() const override;
    int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;
    int runModifier() const override;
    int chargeModifier() const override;

private:

    WeaponOption m_weaponOption = ReaverBlades;
    bool m_iconBearer = false;
    bool m_hornblower = false;

    Weapon m_reaverBlades,
        m_reaverBladesChieftain,
        m_meatripperAxe,
        m_meatripperAxeChieftain;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Hornblower                       Yes
// Icon Bearer                      Yes
// Frenzied Devotion                Yes
// Reaver Blades                    Yes
//

} // namespace Khorne

#endif //BLOODREAVERS_H