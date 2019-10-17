/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ARCHAON_H
#define ARCHAON_H

#include <Unit.h>
#include <Weapon.h>

namespace Everchosen
{

class Archaon : public Unit
{
public:
    static const int BASESIZE = 130;
    static const int WOUNDS = 20;
    static const int POINTS_PER_UNIT = 660;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    Archaon();
    ~Archaon() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

    int move() const override;

protected:

    void onWounded() override;
    int getDamageTableIndex() const;

    Wounds computeReturnedDamage(const Weapon *weapon, int saveRoll) const override;
    Wounds applyWoundSave(const Wounds &wounds) override;
    void onStartCombat(PlayerId player) override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

private:

    mutable int m_slayerOfKingsSixesThisCombat = 0;

    Weapon m_slayerOfKings,
        m_dorgharsClaws,
        m_dorgharsTails,
        m_dorgharsHeads;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// The Eye of Sheerian              No
// The Slayer of Kings              Yes
// The Armour of Morkar             Yes
// Chaos Runeshield                 Yes
// The Crown of Domination          No
// Triple-headed Monstrosity        No
//   Filth-spewer                   No
//   Skull-gorger                   No
//   Spell-eater                    No
// The Everchosen                   No
// Warlord Without Equal            No
//

} // namespace Everchosen

#endif //ARCHAON_H