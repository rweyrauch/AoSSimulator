/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SPORESPLATTAFANATICS_H
#define SPORESPLATTAFANATICS_H

#include <gloomspitegitz/GloomspiteGitzBase.h>
#include <Weapon.h>

namespace GloomspiteGitz {

    class SporesmashaFanatics : public GloomspiteGitzBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        SporesmashaFanatics();

        ~SporesmashaFanatics() override = default;

        bool configure(int numModels);

    protected:

    private:

        Weapon m_ballAndChain;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Puffshroom Frenzy                TODO
// Spore Cloud                      TODO
// Splat!                           TODO
// Whirling Death                   TODO
//

} // namespace GloomspiteGitz

#endif // SPORESPLATTAFANATICS_H