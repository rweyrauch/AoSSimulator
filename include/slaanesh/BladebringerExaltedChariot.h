/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BLADEBRINGEREXALTEDCHARIOT_H
#define BLADEBRINGEREXALTEDCHARIOT_H

#include <slaanesh/SlaaneshBase.h>
#include <Weapon.h>

namespace Slaanesh {

    class BladebringerOnExaltedChariot : public SlaaneshBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        BladebringerOnExaltedChariot();

        ~BladebringerOnExaltedChariot() override = default;

        bool configure(Lore lore);

    protected:

        void onCharged() override;

        void onStartCombat(PlayerId player) override;

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

    private:

        int m_extraMeleeAttacks = 0;

        Weapon m_flensingWhips,
                m_piercingClaws,
                m_poisonedTongues;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Excess of Blades                 Yes
// Pungent Soulscent                Yes
// Acquiescence                     TODO
//

} // Slannesh

#endif //BLADEBRINGEREXALTEDCHARIOT_H