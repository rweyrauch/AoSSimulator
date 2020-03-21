/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CORPSECARTLODESTONE_H
#define CORPSECARTLODESTONE_H

#include <death/LegionOfNagash.h>
#include <Weapon.h>

namespace Death {

    class CorpseCartWithUnholyLodestone : public LegionOfNagashBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        CorpseCartWithUnholyLodestone();

        ~CorpseCartWithUnholyLodestone() override;

        bool configure();

    protected:

        int unholyLodestoneCastingMod(const Unit* caster);

    private:

        Weapon m_goad,
            m_lash,
            m_blades;

        lsignal::slot m_lodestoneSlot;

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Unholy Lodestone                 Yes
// Locus of Undeath                 TODO
// Cryptsword                       TODO
//

} // namespace Death

#endif // CORPSECARTLODESTONE_H