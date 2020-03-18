/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CRYPTHAUNTERCOURTIER_H
#define CRYPTHAUNTERCOURTIER_H

#include <fec/FleshEaterCourts.h>
#include <Weapon.h>

namespace FleshEaterCourt
{

class CryptHaunterCourtier : public FleshEaterCourts
{
public:
    static const int BASESIZE = 50;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 120;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    CryptHaunterCourtier();
    ~CryptHaunterCourtier() override = default;

    bool configure();

protected:

    void onStartHero(PlayerId player) override;
    Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

private:

    Weapon m_massiveBoneClub,
        m_rancidTalons;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Noble Blood                      Yes
// Chosen of the King               Yes
// Muster King's Chosen             TODO
//

} // namespace FleshEaterCourt

#endif //CRYPTHAUNTERCOURTIER_H