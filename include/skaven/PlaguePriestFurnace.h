/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef PLAGUEPRIESTFURNACE_H
#define PLAGUEPRIESTFURNACE_H

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven {

    class PlaguePriestOnPlagueFurnace : public Skaventide {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        PlaguePriestOnPlagueFurnace();

        ~PlaguePriestOnPlagueFurnace() override;

        bool configure();

    protected:

        void onWounded() override;

        void onRestore() override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onEndCombat(PlayerId player) override;

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnit) override;

        void onStartHero(PlayerId player) override;

        int getDamageTableIndex() const;

        int altarOfTheHornedRat(const Unit *unit);

    private:

        Weapon m_censer,
                m_staff,
                m_blades,
                m_spikes;

        lsignal::slot m_connection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Altar of the Horned Rat          Yes
// Great Plague Censer              Yes
// Noxious Prayers                  Partial/TODO
//   Filth-filth!                   Yes
//   Rabid-rabid!                   Yes
// Poisonous Fumes                  Yes
// Protection of the Horned Rat     Yes
// Pushed into Battle               Partial/TODO
//

} // namespace Skaven

#endif //PLAGUEPRIESTFURNACE_H