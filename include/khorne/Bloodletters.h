/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BLOODLETTERS_H
#define BLOODLETTERS_H

#include <Unit.h>
#include <Weapon.h>

namespace Khorne
{

class Bloodletters : public Unit
{
public:
    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 110;
    static const int POINTS_MAX_UNIT_SIZE = 300;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    Bloodletters();
    ~Bloodletters() override = default;

    bool configure(int numModels, bool iconBearer, bool standardBearer, bool hornblowers);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    int extraAttacks(const Weapon* weapon) const override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    Rerolls toHitRerolls(const Weapon* weapon, const Unit* target) const override;
    void computeBattleshockEffect(int roll, int& numFled, int& numAdded) const override;
    void restoreModels(int numModels) override;

private:

    bool m_iconBearer = false;
    bool m_standarBearer = false;
    bool m_hornblower = false;

    Weapon m_hellblade,
        m_hellbladeReaper;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Hornblower                       No
// Icon Bearer                      Yes
// Standard Bearer                  No
// Decapitating Blow                Yes
// Locus of Fury                    Yes
// Murderous Tide                   Yes
//

} // namespace Khorne

#endif //BLOODLETTERS_H