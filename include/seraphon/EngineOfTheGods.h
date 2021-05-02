/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon {

    class EngineOfTheGods : public SeraphonBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        EngineOfTheGods(WayOfTheSeraphon way, Constellation constellation, CommandTrait trait, Artefact artefact, bool isGeneral);

        EngineOfTheGods() = delete;

        ~EngineOfTheGods() override;

    protected:

        void onWounded() override;

        void onRestore() override;

        size_t getDamageTableIndex() const;

        void onCharged() override;

        void onStartShooting(PlayerId player) override;

        void onStartCombat(PlayerId player) override;

        int targetSaveModifier(const Weapon *weapon, const Unit *attacker) const override;

        Rerolls steadfastMajestyBraveryReroll(const Unit *unit);

        Rerolls cosmicEngineChargeReroll(const Unit *unit);

        int cosmicEngineAttackMod(const Unit *attacker, const Model *attackingModel, const Weapon *weapon, const Unit *target);

    private:

        Weapon  m_javelins{Weapon::Type::Missile, "Meteoric Javelins", 8, 4, 5, 4, 0, 1},
                m_horns{Weapon::Type::Melee, "Massive Horns", 2, 2, 3, 3, -1, 4},
                m_jaws{Weapon::Type::Melee, "Grinding Jaws", 1, 2, 3, 3, -1, 2},
                m_stomps{Weapon::Type::Melee, "Crushing Stomps", 1, 5, 3, 3, -1, 2};

        lsignal::slot m_steadfastSlot,
                m_cosmicEngineChargeSlot,
                m_cosmicEngineAttackSlot;

        bool m_timeStoodStill = false;
        Unit *m_armouredCrestAttacker = nullptr;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Armoured Crest                   Yes
// Unstoppable Stampede             Yes
// Steadfast Majesty                Yes
// Cosmic Engine                    Yes
//

} // namespace Seraphon
