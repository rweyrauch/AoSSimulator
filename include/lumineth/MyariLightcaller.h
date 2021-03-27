/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <lumineth/LuminethRealmLords.h>
#include <Weapon.h>

namespace LuminethRealmLords {

    class MyariLigthcaller : public LuminethBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        MyariLigthcaller();

        ~MyariLigthcaller() override;

        void enableDazzlingLight() {
            m_lightConnection.set_lock(false);
        }

    protected:

        void configure(Lore lore);

        void onStartHero(PlayerId player) override;

        int castingModifier() const override {
            // Scryowl Familiar
            return (LuminethBase::castingModifier() + 1);
        }

        int unbindingModifier() const override {
            // Scryowl Familiar
            return (LuminethBase::castingModifier() + 1);
        }

        int dazzlingLight(const Unit *attacker, const Weapon *weapon, const Unit *target);

    private:

        lsignal::slot m_lightConnection;

        Weapon m_beams,
            m_staff;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Scryowl Familar                  Partial
// Dazzling Light                   Yes
//

} // namespace LuminethRealmLords

