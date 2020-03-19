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

    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 120;
    static const int POINTS_MAX_UNIT_SIZE = 300;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    SistersOfSlaughter();
    ~SistersOfSlaughter() override = default;

    bool configure(int numModels, bool sacrificialKnife, bool hornblowers, bool standardBearers);

protected:

    int rollBattleshock() const override;
    int toSaveModifier(const Weapon *weapon) const override;
    Wounds computeReturnedDamage(const Weapon* weapon, int saveRoll) const override;

protected:

    bool m_hasSacrificialKnife = true;
    bool m_hornblowers = false;
    bool m_standardBearers = false;

private:

    Weapon m_sacrificialKnife,
        m_sacrificialKnifeHandmaiden,
        m_barbedWhip,
        m_barbedWhipHandmaiden;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Dance of Death                   Yes
// Bladed Bucklers                  Yes
//

} // namespace DaughtersOfKhaine

#endif //SISTERSOFSLAUGHTER_H