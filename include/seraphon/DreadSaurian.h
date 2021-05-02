/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon {

    class DreadSaurian : public SeraphonBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        DreadSaurian(WayOfTheSeraphon way, Constellation constellation);

        DreadSaurian() = delete;

        ~DreadSaurian() override;

    protected:

        void onWounded() override;

        void onRestore() override;

        void onCharged() override;

        void onFriendlyUnitSlain(const Unit *attacker) override;

        size_t getDamageTableIndex() const;

        int terror(const Unit *target);

    private:

        Weapon  m_gargantuanJaws{Weapon::Type::Melee, "Gargantuan Jaws", 2, 3, 4, 3, -2, 6},
                m_rakingClaws{Weapon::Type::Melee, "Raking Claws", 2, 6, 2, 3, -1, 2},
                m_armouredTail{Weapon::Type::Melee, "Armoured Tail", 2, 1, 4, 3, -1, RAND_D6};

        lsignal::slot m_terrorSlot;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Arcane Glyphs                    TODO
// Obliterating Charge              Yes
// Death Throes                     Yes
// Terror                           Yes
// Roar of Ruin                     TODO
//

} // namespace Seraphon

