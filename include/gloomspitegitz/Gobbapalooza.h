/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef GOBBAPALOOZA_H
#define GOBBAPALOOZA_H

#include <gloomspitegitz/GloomspiteGitzBase.h>
#include <Weapon.h>
#include <gloomspitegitz/GloomspiteSpells.h>

namespace GloomspiteGitz {

    class Gobbapalooza : public GloomspiteGitzBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(int numModels);

        Gobbapalooza();

        ~Gobbapalooza() override = default;

        bool configure();

    protected:

        int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;

        int toSaveModifier(const Weapon *weapon, const Unit* attacker) const override;

    private:

        Weapon m_tusksAndFangs,
                m_stikka,
                m_scorpisquigStikka,
                m_staff,
                m_knife;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Hallucinogenic Fungus Brews      Yes
// Bogeyman                         TODO
// Splippery Git                    Yes
// Loonshrine Potion                TODO
// Poison Brewer                    TODO
// Mesmerise                        TODO
// Fungoid Cloud                    TODO
//

} // namespace GloomspiteGitz

#endif //GOBBAPALOOZA_H