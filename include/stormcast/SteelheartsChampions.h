/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef STEELHEARTSCHAMPIONS_H
#define STEELHEARTSCHAMPIONS_H

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>
#include <UnitFactory.h>

namespace StormcastEternals
{

class SteelheartsChampions : public StormcastEternal
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 2;
    static const int POINTS_PER_UNIT = 100;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    SteelheartsChampions();
    ~SteelheartsChampions() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

    int toHitModifier(const Weapon* weapon, const Unit* unit) const override;
    Rerolls toSaveRerolls(const Weapon* weapon) const override;
    Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

private:

    Weapon m_broadSword,
        m_grandhammer,
        m_warhammer;

    static bool s_registered;

};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Severin Steelheart               Yes
// Lay Low the Tyrants              Yes
// Heroic Guard                     No
// Sigmarite Shields                Yes
//

} // namespace StormcastEternals

#endif //STEELHEARTSCHAMPIONS_H