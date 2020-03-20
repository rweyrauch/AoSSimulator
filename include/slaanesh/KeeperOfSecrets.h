/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef KEEPEROFSECRETS_H
#define KEEPEROFSECRETS_H

#include <slaanesh/SlaaneshBase.h>
#include <Weapon.h>

namespace Slaanesh
{

class KeeperOfSecrets : public SlaaneshBase
{
public:

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
    static int ComputePoints(int numModels);

    KeeperOfSecrets();
    ~KeeperOfSecrets() override = default;

    bool configure(WeaponOption weapon);

protected:

    int getDamageTableIndex() const;
    void onWounded() override;
    void onRestore() override;
    void onStartCombat(PlayerId player) override;
    Wounds onEndCombat(PlayerId player) override;
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
// Abilities                    Implemented
// -------------------------------------------
// Ritual Knife                     Yes
// Dark Temptations                 Partial/TODO
// Delicate Precision               Yes
// Living Whip                      TODO
// Shining Aegis                    Yes
// Sinistrous Hand                  Partial/TODO
// Cacophonic Choir                 TODO
// Excess of Violence               TODO
//

} // Slannesh

#endif //KEEPEROFSECRETS_H