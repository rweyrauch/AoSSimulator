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

namespace Sylvaneth {

    class YltharisGuardians : public SylvanethBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        YltharisGuardians();

        ~YltharisGuardians() override = default;

        bool configure();

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
// Abilities                    Implemented
// -------------------------------------------
// Martial Memories                 TODO
// Vigour and Wrath                 Yes
//

} // namespace Sylvaneth

#endif //YLTHARISGUARDIANS_H