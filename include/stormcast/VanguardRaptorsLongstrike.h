/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef VANGUARDRAPTORSLONGSTRIKE_H
#define VANGUARDRAPTORSLONGSTRIKE_H

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals
{

class VanguardRaptorsLongstrike : public StormcastEternal
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 180;
    static const int POINTS_MAX_UNIT_SIZE = POINTS_PER_BLOCK * 4;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    VanguardRaptorsLongstrike();
    ~VanguardRaptorsLongstrike() override
    {
        delete m_pLongstikeCrossbow;
    }

    bool configure(int numModels);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    int generateMortalWounds(const Weapon* weapon, const Unit* unit, const Hits& hits, const WoundingHits& wounds) override;
    void onStartShooting(PlayerId player) override;

private:

    Weapon* m_pLongstikeCrossbow = nullptr;

    static Weapon s_longstikeCrossbow,
                  s_heavyStock,
                  s_beakAndClaws;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Headshot                         Yes
// Hunting Call                     No
// Longshot                         Yes
//

} // namespace StormcastEternals

#endif //VANGUARDRAPTORSLONGSTRIKE_H