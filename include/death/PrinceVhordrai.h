/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef PRINCEVHORDRAI_H
#define PRINCEVHORDRAI_H

#include <death/LegionOfNagash.h>
#include <Weapon.h>

namespace Death {

    class PrinceVhordrai : public LegionOfNagashBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        PrinceVhordrai();

        ~PrinceVhordrai() override = default;

        bool configure();

    protected:

        void onWounded() override;

        void onRestore() override;

        int getDamageTableIndex() const;

    private:

        Weapon m_bloodlance,
            m_maw,
            m_claws;

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// The Hunger                       TODO
// Chalice of Blood                 TODO
// Bloodlance Charge                TODO
// Breath of Shyish                 TODO
// Deathly Invocation               TODO
// Quickblood                       TODO
// Fist of Nagash                   TODO
//

} // namespace Death

#endif // PRINCEVHORDRAI_H