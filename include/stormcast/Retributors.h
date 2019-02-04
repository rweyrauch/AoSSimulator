/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef RETRIBUTORS_H
#define RETRIBUTORS_H

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals
{

class Retributors : public StormcastEternal
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 3;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 220;
    static const int POINTS_MAX_UNIT_SIZE = POINTS_PER_BLOCK * 4;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    Retributors();
    ~Retributors() override = default;

    bool configure(int numModels, int numStarsoulMaces);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    int generateMortalWounds(const Weapon* weapon, const Unit* unit, const Hits& hits, const WoundingHits& wounds) override;

private:

    Weapon m_lightningHammer,
                  m_lightningHammerPrime,
                  m_starsoulMace;

    static bool s_registered;
};

} // namespace StormcastEternals

#endif //RETRIBUTORS_H