/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SPIDERFLINGER_H
#define SPIDERFLINGER_H

#include <gloomspitegitz/GloomspiteGitzBase.h>
#include <Weapon.h>

namespace GloomspiteGitz {

    class ArachnarokSpiderWithFlinger : public GloomspiteGitzBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        ArachnarokSpiderWithFlinger();

        ~ArachnarokSpiderWithFlinger() override = default;

        bool configure();

    protected:

        void onWounded() override;

        void onRestore() override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        int getDamageTableIndex() const;

        Weapon m_spiderBows,
                m_flinger,
                m_chitinousLegs,
                m_monstrousFangs,
                m_crookedSpears;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Flinger                          TODO
// Spider Venom                     Yes
// Wall Crawler                     Yes
//

} // namespace GloomspiteGitz

#endif //SPIDERFLINGER_H