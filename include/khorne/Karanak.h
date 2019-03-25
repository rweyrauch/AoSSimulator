/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef KARANAK_H
#define KARANAK_H

#include <Unit.h>
#include <Weapon.h>

namespace Khorne
{

class Karanak : public Unit
{
public:
    static const int BASESIZE = 60; // x35 oval
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 140;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    Karanak();
    ~Karanak() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    void onBeginTurn(int battleRound) override;

    // Unflagging Hunter
    Rerolls chargeRerolls() const override { return RerollFailed; }

    Rerolls toHitRerolls(const Weapon* weapon, const Unit* target) const override;
    Rerolls toWoundRerolls(const Weapon* weapon, const Unit* target) const override;

private:

    Unit* m_pQuarry = nullptr;

    Weapon m_goreSlickClaws,
        m_savageMaws;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Brass Collar of Bloody Vengeance No
// Prey of the Blood God            Yes
// Unflagging Hunter                Yes
// Call of the Hunt                 No
//

} // namespace Khorne

#endif //KARANAK_H