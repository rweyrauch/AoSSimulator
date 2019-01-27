/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BESTIGORS_H
#define BESTIGORS_H

#include <Unit.h>
#include <Weapon.h>

namespace BeastsOfChaos
{

class Bestigors : public Unit
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 120;
    static const int POINTS_MAX_UNIT_SIZE = 300;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    Bestigors();
    ~Bestigors() override = default;

    bool configure(int numModels, bool brayhorn, bool bannerBearer);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    int toHitModifier(const Weapon* weapon, const Unit* unit) const override;
    Rerolls toHitRerolls(const Weapon* weapon, const Unit* unit) const override;
    int extraAttacks(const Weapon* weapon) const override;
    int runModifier() const override;

private:

    bool m_brayhorn = false;
    bool m_bannerBearer = false;

    static Weapon s_despoilerAxe,
        s_despoilerAxeGougeHorn;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Despoilers                       Yes
// Beastial Charge                  Yes
//

} // namespace BeastsOfChaos

#endif //BESTIGORS_H