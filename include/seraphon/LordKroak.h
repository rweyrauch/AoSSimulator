/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon {

    class LordKroak : public SeraphonBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        LordKroak(WayOfTheSeraphon way, Constellation constellation, bool isGeneral);

        ~LordKroak() override = default;

    protected:

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        Wounds applyWoundSave(const Wounds &wounds, Unit *attackingUnit) override;

        void onStartHero(PlayerId player) override;

        int castingModifier() const override;

        int unbindingModifier() const override;

    private:

        Weapon m_barrier{Weapon::Type::Melee, "Azyrite Force Barrier", 3, 0, 3, 3, -1, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Arcane Vassal                    TODO
// Azyrite Force Barrier            Yes
// Dead for Innumerable Ages        TODO
// Impeccable Foresight             Yes
// Supreme Master of Order          Yes
// Celestial Deliverance            Yes
// Comet's Call                     Yes
// Supreme Gift from the Heavens    TODO
//

} // namespace Seraphon
