/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CORPSECARTBRAZIER_H
#define CORPSECARTBRAZIER_H

#include <death/LegionOfNagash.h>
#include <Weapon.h>

namespace Death {

    class CorpseCartWithBalefireBrazier : public LegionOfNagashBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        CorpseCartWithBalefireBrazier();

        ~CorpseCartWithBalefireBrazier() override = default;

        bool configure();

    protected:

        void onWounded() override;

        void onRestore() override;

        int getDamageTableIndex() const;

    private:

        Weapon m_goad,
            m_lash,
            m_blades;

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Balefire Brazier                 TODO
// Malefic Fumes                    TODO
// Cryptsword                       TODO
//

} // namespace Death

#endif // CORPSECARTBRAZIER_H