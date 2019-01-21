/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BLOODREAVERS_H
#define BLOODREAVERS_H

#include <Unit.h>
#include <Weapon.h>

namespace Khorne
{

class Bloodreavers : public Unit
{
public:
    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 40;

    enum WeaponOption
    {
        ReaverBlades,
        MeatripperAxe
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter& parameter);
    static void Init();

    Bloodreavers();
    ~Bloodreavers() override = default;

    bool configure(int numModels, WeaponOption weapons, bool iconBearer, bool hornblowers);

protected:

    Rerolls toHitRerolls(const Weapon* weapon, const Unit* unit) const override;
    int battlshockModifier() const override;

    WeaponOption m_weaponOption = ReaverBlades;
    bool m_iconBearer = false;
    bool m_hornblower = false;

    static Weapon s_reaverBlades,
                  s_reaverBladesChieftain,
                  s_meatripperAxe,
                  s_meatripperAxeChieftain;

    static bool s_registered;
};

} // namespace Khorne

#endif //BLOODREAVERS_H