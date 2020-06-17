/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BLADEBRINGERSEEKERCHARIOT_H
#define BLADEBRINGERSEEKERCHARIOT_H

#include <slaanesh/SlaaneshBase.h>
#include <Weapon.h>

namespace Slaanesh {

    class BladebringerOnSeekerChariot : public SlaaneshBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        BladebringerOnSeekerChariot();

        ~BladebringerOnSeekerChariot() override = default;

        bool configure(Lore lore);

    protected:

        void onCharged() override;

    private:

        Weapon m_flensingWhips,
                m_piercingClaws,
                m_poisonedTongues;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Impossibly Swift                 Yes
// Multilating Blades               Yes
// Acquiescence                     TODO
//

} // Slannesh

#endif //BLADEBRINGERSEEKERCHARIOT_H