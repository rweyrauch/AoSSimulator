/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <tzeentch/TzeentchBase.h>
#include <Weapon.h>

namespace Tzeentch {

    class Fateskimmer : public TzeentchBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Fateskimmer(ChangeCoven coven, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~Fateskimmer() override = default;

    protected:

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int rollCasting(UnmodifiedCastingRoll &unmodifiedRoll) const override;

        void onRestore() override {
            TzeentchBase::onRestore();
            m_usedArcaneTome = false;
        }

    private:

        mutable bool m_usedArcaneTome = false;

        Weapon  m_magicalFlames{Weapon::Type::Missile, "Magical Flames", 18, 3, 4, 4, -1, 1},
                m_staff{Weapon::Type::Melee, "Staff of Change", 2, 1, 4, 3, -1, RAND_D3},
                m_dagger{Weapon::Type::Melee, "Ritual Dagger", 1, 2, 4, 4, 0, 1},
                m_bite{Weapon::Type::Melee, "Lamprey Bite", 1, 6, 4, 3, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Arcane Tome                      Yes
// Sky-shark                        Yes
// Wake of Fire                     TODO
// Tzeentch's Firestorm             Yes
//

} // namespace Tzeentch

