/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <fyreslayers/Fyreslayer.h>
#include <Weapon.h>

namespace Fyreslayers {

    class GrimwrathBerzerker : public Fyreslayer {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        GrimwrathBerzerker(Lodge lodge, Artefact artefact);

        GrimwrathBerzerker() = delete;

        ~GrimwrathBerzerker() override = default;

    protected:

        void onFriendlyModelSlain(int numSlain, Unit *attacker, Wounds::Source source) override;

        void onEndCombat(PlayerId player) override;

    private:

        Weapon  m_throwingAxe{Weapon::Type::Missile, "Fyresteel Throwing Axe", 8, 1, 4, 4, -1, 1},
                m_greatAxe{Weapon::Type::Melee, "Fyrestorm Greataxe", 1, 4, 3, 3, -2, 2};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Battle-fury                      Yes
// Dead, But Not Defeated           Yes
// Grimwrath Oath "I will..."       TODO
//  "cut down the priests of Grimnir's enemies"
//  "guard them with my life"
//  "let nothing stand in my way"
//  "prove Grimnir's might to our allies"
//  "not be stopped"
//  "strike hard and true in Grimnar's name"
//

} // namespace Fyreslayers
