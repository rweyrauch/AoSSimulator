/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <dok/DaughterOfKhaine.h>
#include <Weapon.h>

namespace DaughtersOfKhaine {

    class WitchAelves : public DaughterOfKhaine {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        WitchAelves(int points);

        ~WitchAelves() override = default;

        bool configure(int numModels, bool pairedKnives, bool hornblowers, bool standardBearers);

    protected:

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        int rollBattleshock() const override;

        int toSaveModifier(const Weapon *weapon, const Unit *attacker) const override;

        Wounds computeReturnedDamage(const Weapon *weapon, int saveRoll) const override;

    protected:

        bool m_pairedKnives = true;

    private:

        Weapon m_sacrificialKnife,
                m_sacrificialKnifeHag;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Paired Sacrificial Knives        Yes
// Frenzied Fervour                 Yes
// Bladed Bucklers                  Yes
//

} // namespace DaughtersOfKhaine
