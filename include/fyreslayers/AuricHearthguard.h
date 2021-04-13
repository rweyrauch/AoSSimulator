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

    class AuricHearthguard : public Fyreslayer {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        AuricHearthguard();

        ~AuricHearthguard() override = default;

        bool configure(int numModels);

    protected:

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        Weapon m_magmapike,
                m_magmapikeKarl,
                m_throwingAxe,
                m_magmapikeMelee;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Molten Rockbolts                 Partial/TODO
// Sworn Protectors                 TODO
//

} // namespace Fyreslayers
