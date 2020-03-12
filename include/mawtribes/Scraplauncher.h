/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SCRAPLAUNCHER_H
#define SCRAPLAUNCHER_H

#include <mawtribes/MawtribesBase.h>
#include <Weapon.h>

namespace OgorMawtribes
{

class GnoblarScraplauncher : public MawtribesBase
{
public:

    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 9;
    static const int POINTS_PER_UNIT = 120;

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);
    static void Init();
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };

    GnoblarScraplauncher();
    ~GnoblarScraplauncher() override = default;

    bool configure();

protected:

    int toHitModifier(const Weapon *weapon, const Unit *target) const override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

private:

    Weapon m_scrap,
        m_scrapperWeapons,
        m_horns;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Deadly Rain of Scrap             Yes
// Rhinox Charge                    Yes
//

} // namespace OgorMawtribes

#endif //SCRAPLAUNCHER_H
