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

    class AlarithSpiritOfTheMountain : public LuminethBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        AlarithSpiritOfTheMountain();

        ~AlarithSpiritOfTheMountain() override = default;

        bool configure();

    protected:

        void onWounded() override;

        void onRestore() override;

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        void onStartShooting(PlayerId player) override;
        void onStartCombat(PlayerId player) override;

    private:

        size_t getDamageTableIndex() const;

        Weapon m_blast,
            m_hammer,
            m_hooves;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// All but Immovable                Yes
// Ponderous Advice                 TODO
// Stoneheart Shockwave             Yes
// Stonemage Symbiosis              Yes
// Faith of the Mountain            TODO
//

} // namespace LuminethRealmLords
