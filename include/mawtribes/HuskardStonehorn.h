/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef HUSKARDSTONEHORN_H
#define HUSKARDSTONEHORN_H

#include <mawtribes/MawtribesBase.h>
#include <Weapon.h>

namespace OgorMawtribes
{

class HuskardOnStonehorn : public MawtribesBase
{
public:

    enum WeaponOption
    {
        HarpoonLauncher,
        Chaintrap,
        BloodVulture
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);
    static int ComputePoints(int numModels);
    static void Init();

    HuskardOnStonehorn();
    ~HuskardOnStonehorn() override = default;

    bool configure(WeaponOption option);

protected:

    int getDamageTableIndex() const;
    void onWounded() override;
    void onRestore() override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    Wounds applyWoundSave(const Wounds& wounds) override;
    void onStartShooting(PlayerId player) override;

private:

    WeaponOption m_option = HarpoonLauncher;

    Weapon m_harpoon,
        m_chaintrap,
        m_vulture,
        m_kicks,
        m_horns,
        m_hooves;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Earth-shattering Charge          Yes
// Stone Skeleton                   Yes
// Blood Vulture                    Yes
// Line-breakers                    TODO
//

} // namespace OgorMawtribes

#endif //HUSKARDSTONEHORN_H
