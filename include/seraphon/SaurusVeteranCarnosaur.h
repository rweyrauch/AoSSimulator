/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SAURUSVETERANCARNOSAUR_H
#define SAURUSVETERANCARNOSAUR_H

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon
{

class SaurusScarVeteranOnCarnosaur : public SeraphonBase
{
public:

    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 210;

    enum WeaponOption
    {
        CelestiteWarblade,
        CelestiteWarspear,
        CelestiteGreatblade
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; }
    static void Init();

    SaurusScarVeteranOnCarnosaur();
    ~SaurusScarVeteranOnCarnosaur() override = default;

    bool configure(WeaponOption option);

    int move() const override;

protected:

    void onWounded() override;
    int getDamageTableIndex() const;
    int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;
    int toHitModifier(const Weapon *weapon, const Unit *target) const override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

private:

    Weapon m_warblade,
        m_warspear,
        m_greatblade,
        m_forelimbs,
        m_jaws;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Blood Frenzy                     No
// Celestite Warspear               Yes
// Cold Ferocity                    Yes
// Pinned Down                      Yes
// Terror                           No
// Saurian Savagery                 No
//

} // namespace Seraphon

#endif //SAURUSOLDBLOODCARNOSAUR_H