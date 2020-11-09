/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef MELUSAIIRONSCALE_H
#define MELUSAIIRONSCALE_H

#include <dok/DaughterOfKhaine.h>
#include <Weapon.h>

namespace DaughtersOfKhaine {

    class MelusaiIronscale : public DaughterOfKhaine {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        MelusaiIronscale();

        ~MelusaiIronscale() override = default;

        bool configure();

    protected:

    private:

        Weapon m_keldrisaithShooting,
                m_keldrisaith;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Blood of the Oracle              TODO
// Gory Offering                    TODO
// Turned to Crystal                TODO
// Wrath of the Scathborn           TODO
//

} // namespace DaughtersOfKhaine

#endif //MELUSAIIRONSCALE_H