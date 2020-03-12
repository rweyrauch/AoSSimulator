/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef PLAGUEPRIESTONPLAGUEFURNACE_H
#define PLAGUEPRIESTONPLAGUEFURNACE_H

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven
{

class PlaguePriestOnPlagueFurnace : public Skaventide
{
public:
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 13;
    static const int POINTS_PER_UNIT = 200;

    static Unit *Create(const ParameterList &parameters);

    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };

    static void Init();

    PlaguePriestOnPlagueFurnace();
    ~PlaguePriestOnPlagueFurnace() override;

    bool configure();

protected:

    void onWounded() override;
    void onRestore() override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    Wounds onEndCombat(PlayerId player) override;
    int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;
    Wounds applyWoundSave(const Wounds &wounds) override;
    void onStartHero(PlayerId player) override;

    int getDamageTableIndex() const;

    int altarOfTheHornedRat(const Unit* unit);

private:

    Weapon m_censer,
        m_staff,
        m_blades,
        m_spikes;

    lsignal::slot m_connection;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Altar of the Horned Rat          Yes
// Great Plague Censer              Yes
// Noxious Prayers                  Partial
//   Filth-filth!                   Yes
//   Rabid-rabid!                   Yes
// Poisonous Fumes                  Yes
// Protection of the Horned Rat     Yes
// Pushed into Battle               Partial
//

} // namespace Skaven

#endif //PLAGUEPRIESTONPLAGUEFURNACE_H