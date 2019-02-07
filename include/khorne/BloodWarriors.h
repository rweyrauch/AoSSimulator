/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BLOODWARRIORS_H
#define BLOODWARRIORS_H

#include <Unit.h>
#include <Weapon.h>

namespace Khorne
{

class BloodWarriors : public Unit
{
public:
    static const int BASESIZE = 32;
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 100;
    static const int POINTS_MAX_UNIT_SIZE = 520;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    BloodWarriors();
    ~BloodWarriors() override = default;

    bool configure(int numModels, bool pairedGoreax, int numGoreglaives, bool iconBearer);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    Rerolls toHitRerolls(const Weapon* weapon, const Unit* unit) const override;
    int battlshockModifier() const override;
    Wounds computeReturnedDamage(const Weapon* weapon, int saveRoll) const override;

private:

    bool m_pairedGoreaxe = false;
    bool m_iconBearer = false;

    Weapon m_goreaxe,
                  m_goreaxeChampion,
                  m_goreglaive;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Icon Bearer                      Yes
// No Respite                       No
// Goreaxes                         Yes
// Gorefists                        Yes

} // namespace Khorne

#endif //BLOODWARRIORS_H