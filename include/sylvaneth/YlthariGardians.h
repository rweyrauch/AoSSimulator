/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef YLTHARISGUARDIANS_H
#define YLTHARISGUARDIANS_H

#include <sylvaneth/SylvanethBase.h>
#include <Weapon.h>

namespace Sylvaneth
{

class YltharisGuardians : public SylvanethBase
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int POINTS_PER_UNIT = 0;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    YltharisGuardians();
    ~YltharisGuardians() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

    Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

protected:

private:

    Weapon m_enchantedGreatBlade,
        m_revenantBow,
        m_protectorGlaive,
        m_revenantBowMelee;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Martial Memories                 No
// Vigour and Wrath                 Yes
//

} // namespace Sylvaneth

#endif //YLTHARISGUARDIANS_H