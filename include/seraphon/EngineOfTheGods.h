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

        static int ComputePoints(int numModels);

        static void Init();

        EngineOfTheGods();

        ~EngineOfTheGods() override;

        bool configure();

    protected:

        void onWounded() override;

        void onRestore() override;

        int getDamageTableIndex() const;

        void onCharged() override;

        void onStartShooting(PlayerId player) override;

        void onStartCombat(PlayerId player) override;

        int targetSaveModifier(const Weapon *weapon, const Unit *attacker) const override;

        Rerolls steadfastMajestyBraveryReroll(const Unit *unit);

        Rerolls cosmicEngineChargeReroll(const Unit *unit);

        int cosmicEngineAttackMod(const Unit *attacker, const Model *attackingModel, const Weapon *weapon,
                                  const Unit *target);

    private:

        Weapon m_javelins,
                m_horns,
                m_jaws,
                m_stomps;

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
