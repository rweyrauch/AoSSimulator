/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef DRYADS_H
#define DRYADS_H

#include <sylvaneth/SylvanethBase.h>
#include <Weapon.h>

namespace Sylvaneth
{

class Dryads : public SylvanethBase
{
public:

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    Dryads();
    ~Dryads() override = default;

    bool configure(int numModels);

protected:

    int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;
    int toSaveModifier(const Weapon *weapon) const override;
    void onStartCombat(PlayerId player) override;
    int toHitModifier(const Weapon *weapon, const Unit *target) const override;

private:

    Weapon m_wrackingTalons,
        m_wrackingTalonsNymph;

    Unit* m_enrapturedUnit = nullptr;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Blessing of the Forest           Yes
// Enrapturing Song                 Yes
// Impenetrable Thicket             Yes
//

} // namespace Sylvaneth

#endif //DRYADS_H