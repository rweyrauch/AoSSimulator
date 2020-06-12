/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LORDKROAK_H
#define LORDKROAK_H

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon {

    class LordKroak : public SeraphonBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        LordKroak();

        ~LordKroak() override = default;

        bool configure(Lore lore);

    protected:

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        Wounds applyWoundSave(const Wounds &wounds) override;

        void onStartHero(PlayerId player) override;

        int castingModifier() const override;

    private:

        Weapon m_barrier;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Arcane Vassal                    TODO
// Azyrite Force Barrier            Yes
// Dead for Innumerable Ages        Yes
// Impeccable Foresight             Yes
// Masters of Order                 Yes
// Celestial Deliverance            TODO
// Comet's Call                     TODO
// Gift from the Heavens            TODO
//

} // namespace Seraphon

#endif //LORDKROAK_H