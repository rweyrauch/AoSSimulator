/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SISTERSOFSLAUGHTER_H
#define SISTERSOFSLAUGHTER_H

#include <dok/DaughterOfKhaine.h>
#include <Weapon.h>

namespace DaughtersOfKhaine
{

class SistersOfSlaughter : public DaughterOfKhaine
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    SistersOfSlaughter();
    ~SistersOfSlaughter() override = default;

    bool configure(int numModels, bool sacrificialKnife, bool hornblowers, bool standardBearers);

protected:

    int rollBattleshock() const override;
    int toSaveModifier(const Weapon *weapon) const override;
    Wounds computeReturnedDamage(const Weapon* weapon, const Dice::RollResult& saveRolls) const override;

protected:

    bool m_sacrificialKnife = true;
    bool m_hornblowers = false;
    bool m_standardBearers = false;

private:

    static Weapon s_sacrificialKnife,
        s_sacrificialKnifeHandmaiden,
        s_barbedWhip,
        s_barbedWhipHandmaiden;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Dance of Death                   No
// Bladed Bucklers                  Yes
//

} // namespace DaughtersOfKhaine

#endif //SISTERSOFSLAUGHTER_H