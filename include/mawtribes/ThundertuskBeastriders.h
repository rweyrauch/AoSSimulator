/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef THUNDERTUSKBEASTRIDERS_H
#define THUNDERTUSKBEASTRIDERS_H

#include <mawtribes/MawtribesBase.h>
#include <Weapon.h>

namespace OgorMawtribes
{

class ThundertuskBeastriders : public MawtribesBase
{
public:

    enum WeaponOption
    {
        Chaintrap,
        BloodVulture
    };

    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 300;

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    ThundertuskBeastriders();
    ~ThundertuskBeastriders() override = default;

    bool configure(WeaponOption option);

protected:

    int getDamageTableIndex() const;
    void onWounded() override;
    void onRestore() override;
    int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;
    void onStartShooting(PlayerId player) override;

private:

    WeaponOption m_option = Chaintrap;

    Weapon m_harpoon,
        m_chaintrap,
        m_vulture,
        m_ice,
        m_kicks,
        m_tusks;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Blasts of Frost-wreathed Ice     Yes
// Numbing Chill                    Yes
// Blood Vulture                    Yes
//

} // namespace OgorMawtribes

#endif //THUNDERTUSKBEASTRIDERS_H
