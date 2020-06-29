/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef AVALENOR_H
#define AVALENOR_H

#include <lumineth/LuminethRealmLords.h>
#include <Weapon.h>

namespace LuminethRealmLords {

    class AvalenorTheStoneheartKing : public LuminethBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        AvalenorTheStoneheartKing();

        ~AvalenorTheStoneheartKing() override = default;

        bool configure();

    protected:

        void onWounded() override;

        void onRestore() override;

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        int getDamageTableIndex() const;

        Weapon m_blast,
            m_hammer,
            m_hooves;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// All but Immovable                Yes
// Firestealer Hammers              Yes
// Elder Wisdom                     TODO
// Guardian of Hysh                 TODO
// Stonemage Symbiosis              Yes
// Unshakeable Faith of the Mountain    TODO

} // namespace LuminethRealmLords

#endif // AVALENOR_H
