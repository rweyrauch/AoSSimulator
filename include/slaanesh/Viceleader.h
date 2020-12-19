/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef VICELEADER_H
#define VICELEADER_H

#include <slaanesh/SlaaneshBase.h>
#include <Weapon.h>

namespace Slaanesh {

    class ViceleaderHeraldOfSlaanesh : public SlaaneshBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        ViceleaderHeraldOfSlaanesh();

        ~ViceleaderHeraldOfSlaanesh() override = default;

        bool configure(Lore lore);

    protected:

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnit) override;

    private:

        Weapon m_ravagingClaws;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Lightning Reflexes               Yes
// Lithe and Swift                  Yes
// Acquiescence                     TODO
//

} // Slannesh

#endif //VICELEADER_H