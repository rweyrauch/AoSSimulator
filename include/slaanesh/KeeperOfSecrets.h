/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef KEEPEROFSECRETS_H
#define KEEPEROFSECRETS_H

#include <Unit.h>
#include <Weapon.h>

namespace Slaanesh
{

class KeeperOfSecrets : public Unit
{
public:

    static const int BASESIZE = 100;
    static const int WOUNDS = 14;
    static const int POINTS_PER_UNIT = 360;

    enum WeaponOption
    {
        RitualKnife,
        SinistrousHand,
        LivingWhip,
        ShiningAegis
    };

    static Unit* Create(const ParameterList& parameters);
    static void Init();
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);

    KeeperOfSecrets();
    ~KeeperOfSecrets() override = default;

    bool configure(WeaponOption weapon);
    int move() const override;

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    int getDamageTableIndex() const;
    void onWounded() override;

    void onStartCombat(PlayerId player) override;
    void onEndCombat(PlayerId player) override;
    Wounds applyWoundSave(const Wounds& wounds) override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

private:

    WeaponOption m_weapon = RitualKnife;

    Weapon m_livingWhip,
        m_ritualKnifeOrHand,
        m_greatblade,
        m_impalingClaws;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Ritual Knife                     Yes
// Dark Temptations                 Partial
// Delicate Precision               Yes
// Living Whip                      No
// Shining Aegis                    Yes
// Sinistrous Hand                  Partial
// Cacophonic Choir                 No
// Excess of Violence               No
//

} // Slannesh

#endif //KEEPEROFSECRETS_H