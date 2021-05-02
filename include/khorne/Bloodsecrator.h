/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne {

    class Bloodsecrator : public KhorneBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Bloodsecrator() = delete;

        ~Bloodsecrator() override;

    protected:

        Bloodsecrator(SlaughterHost host, CommandTrait trait, Artefact artefact, bool isGeneral);

        int rageOfKhorneAttackMod(const Unit *attacker, const Model *attackingModel, const Weapon *weapon,
                                  const Unit *target);

        Rerolls loathsomeSorcery(const Unit* caster);

    private:

        Weapon  m_ensorcelledAxe{Weapon::Type::Melee, "Ensorcelled Axe", 1, 4, 3, 3, -1, 1};

        lsignal::slot m_rageOfKhorneSlot, m_loathsomeSorcerySlot;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Loathsome Sorcery                Yes
// Rage of Khorne                   Yes
//

} // namespace Khorne
