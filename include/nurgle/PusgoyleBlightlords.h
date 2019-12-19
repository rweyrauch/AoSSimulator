/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef PUSGOYLEBLIGHTLORDS_H
#define PUSGOYLEBLIGHTLORDS_H

#include <Unit.h>
#include <Weapon.h>

namespace Nurgle
{

class PusgoyleBlightlords : public Unit
{
public:

    static const int BASESIZE = 60;
    static const int WOUNDS = 7;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 200;
    static const int POINTS_MAX_UNIT_SIZE = 800;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    PusgoyleBlightlords();
    ~PusgoyleBlightlords() override = default;

    bool configure(int numModels, int numTocsins);

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

    Wounds applyWoundSave(const Wounds& wounds) override;
    int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;
    void onStartHero(PlayerId player) override;

protected:

private:

    Weapon m_blightedWeapon,
        m_dolorousTocsin,
        m_mouthparts,
        m_venemousSting;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Disgustingly Resilient           Yes
// Virulent Discharge               Yes
// Blighted Weapons                 Yes
//

} // Nurgle

#endif //PUSGOYLEBLIGHTLORDS_H