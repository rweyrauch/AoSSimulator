/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef WRATHBLOODTHIRSTER_H
#define WRATHBLOODTHIRSTER_H

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne
{

class WrathOfKhorneBloodthirster : public KhorneBase
{
public:
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 14;
    static const int POINTS_PER_UNIT = 320;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    WrathOfKhorneBloodthirster();
    ~WrathOfKhorneBloodthirster() override = default;

    bool configure();
    int move() const override;

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    int getDamageTableIndex() const;
    void onWounded() override;
    Rerolls toHitRerolls(const Weapon* weapon, const Unit* target) const override;

private:

    Weapon m_bloodflail,
        m_mightyAxeOfKhorne;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Hellfire Breath                  No
// Relentless Hunter                Yes
// Rune-crown of Khorne             No
// Lord of the Blood Hunt           No
//

} // namespace Khorne

#endif //WRATHBLOODTHIRSTER_H