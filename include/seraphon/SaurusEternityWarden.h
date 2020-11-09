/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SAURUSETERNITYWARDEN_H
#define SAURUSETERNITYWARDEN_H

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon {

    class SaurusEternityWarden : public SeraphonBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        SaurusEternityWarden();

        ~SaurusEternityWarden() override = default;

        bool configure();

    protected:

        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

    private:

        Weapon m_mace,
                m_jaws;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Cold Ferocity                    Yes
// Selfless Protectors              TODO
// Prime Guardian                   TODO
//

} // namespace Seraphon

#endif //SAURUSETERNITYWARDEN_H