/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <sylvaneth/SylvanethBase.h>
#include <Weapon.h>

namespace Sylvaneth {

    class WarsongRevenant : public SylvanethBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        WarsongRevenant(Glade glade, CommandTrait trait, Artefact artefact, bool isGeneral);

        WarsongRevenant() = delete;

        ~WarsongRevenant() override;

    protected:

        Wounds applyWoundSave(const Wounds &wounds, Unit *attackingUnit) override;

        int alariellesSong(const Unit* unit);

        int castingModifier() const override;

        int unbindingModifier() const override;

    private:

        lsignal::slot m_songSlot;

        Weapon m_vines{Weapon::Type::Melee, "Spearing Vines", 3, 5, 3, 3, -1, 2};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Alarielle's Song                 Yes
// Arboreal Cloak                   Yes
// Wyldwood Revenants               Yes
// Unleash Swarms of Spites         Yes
//

} // namespace Sylvaneth
