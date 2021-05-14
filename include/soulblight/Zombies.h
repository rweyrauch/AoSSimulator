/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <soulblight/SoulblightGravelords.h>
#include <Weapon.h>

namespace Soulblight {
    class Zombies : public SoulblightBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Zombies(Legion legion, int numModels, bool standardBearer, bool noiseMaker, int points);

        ~Zombies() override;

    protected:

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        int toWoundModifier(const Weapon *weapon, const Unit *target) const override;

        int rollChargeDistance() override;

        int standardBearerBraveryMod(const Unit *unit);

    private:

        Weapon m_zombieBite;

        lsignal::slot m_standardSlot;

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Standard Bearer                  Yes
// Noise Maker                      Yes
// Dragged Down and Torn Apart      Yes
// The Newly Dead                   TODO
// Vigour Mortis                    Yes
//

} //namespace Soulblight
