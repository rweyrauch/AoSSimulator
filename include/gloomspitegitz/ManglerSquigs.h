/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef MANGLERSQUIGS_H
#define MANGLERSQUIGS_H

#include <gloomspitegitz/GloomspiteGitzBase.h>
#include <Weapon.h>

namespace GloomspiteGitz {

    class ManglerSquigs : public GloomspiteGitzBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        ManglerSquigs();

        ~ManglerSquigs() override = default;

        bool configure();

    protected:

        int toHitModifier(const Weapon *weapon, const Unit *unit) const override;

        void onWounded() override;

        void onFriendlyUnitSlain(const Unit *attacker) override;

        void onRestore() override;

        int getDamageTableIndex() const;

    private:

        Weapon m_hugeFangFilledGob,
                m_ballsAndChains,
                m_grotsBashinStikk;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Ker-splat!                       Yes
// Watch Out!                       Yes
//

} // namespace GloomspiteGitz

#endif //MANGLERSQUIGS_H