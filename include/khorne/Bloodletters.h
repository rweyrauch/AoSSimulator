/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BLOODLETTERS_H
#define BLOODLETTERS_H

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne
{

class Bloodletters : public KhorneBase
{
public:
    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 110;
    static const int POINTS_MAX_UNIT_SIZE = 300;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    Bloodletters();
    ~Bloodletters() override;

    bool configure(int numModels, bool iconBearer, bool standardBearer, bool hornblowers);

protected:

    int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    Rerolls toHitRerolls(const Weapon* weapon, const Unit* target) const override;
    void computeBattleshockEffect(int roll, int& numFled, int& numAdded) const override;
    void restoreModels(int numModels) override;
    Rerolls chargeRerolls() const override;

    Rerolls hornblowerBattleshockReroll(const Unit* unit);

private:

    bool m_iconBearer = false;
    bool m_standarBearer = false;
    bool m_hornblower = false;

    Weapon m_hellblade,
        m_hellbladeReaper;

    lsignal::slot m_hornblowerSlot;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Hornblower                       Yes
// Icon Bearer                      Yes
// Standard Bearer                  Yes
// Decapitating Blow                Yes
// Locus of Fury                    Yes
// Murderous Tide                   Yes
//

} // namespace Khorne

#endif //BLOODLETTERS_H