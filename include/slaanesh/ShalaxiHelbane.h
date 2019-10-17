/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SHALAXIHELBANE_H
#define SHALAXIHELBANE_H

#include <slaanesh/SlaaneshBase.h>
#include <Weapon.h>

namespace Slaanesh
{

class ShalaxiHelbane : public SlaaneshBase
{
public:

    static const int BASESIZE = 100;
    static const int WOUNDS = 14;
    static const int POINTS_PER_UNIT = 340;

    enum WeaponOption
    {
        LivingWhip,
        ShiningAegis
    };

    static Unit* Create(const ParameterList& parameters);
    static void Init();
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);

    ShalaxiHelbane();
    ~ShalaxiHelbane() override = default;

    bool configure(WeaponOption weapon);
    int move() const override;

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

    int getDamageTableIndex() const;
    void onWounded() override;

    Wounds applyWoundSave(const Wounds& wounds) override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

private:

    WeaponOption m_weapon = ShiningAegis;

    Weapon m_livingWhip,
        m_soulpiercer,
        m_impalingClaws;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Cloak of Constriction            No
// Delicate Precision               Yes
// Irresistible Challenge           No
// Living Whip                      No
// Shining Aegis                    Yes
// The Killing Stroke               No
// Refine Senses                    No
//

} // Slannesh

#endif //SHALAXIHELBANE_H