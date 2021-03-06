/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <wanderers/Wanderer.h>
#include <Weapon.h>

namespace Wanderers {

    class EternalGuard : public Wanderer {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        EternalGuard(int numModels, bool standardBearer, bool hornblower, bool gladeShields, int points);

        ~EternalGuard() override = default;

    protected:

        void onStartHero(PlayerId player) override;

        Rerolls runRerolls() const override;

        int braveryModifier() const override;

        Rerolls toSaveRerolls(const Weapon *weapon, const Unit *attacker) const override;

        int toHitModifier(const Weapon *weapon, const Unit *target) const override { return m_fortressModifier; }

        int toWoundModifier(const Weapon *weapon, const Unit *target) const override { return m_fortressModifier; }

        int toSaveModifier(const Weapon *weapon, const Unit *attacker) const override { return m_fortressModifier; }

    private:

        bool m_gladeShields = false;

        int m_fortressModifier = 0;

        Weapon m_spearStave,
                m_spearStaveWarden;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Fortress of Boughs               Yes
// Glade Shields                    Yes
// Standard Bearer                  Yes
// Hornblower                       Yes
//

} // namespace Wanderers
