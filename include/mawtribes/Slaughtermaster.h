/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SLAUGHTERMASTER_H
#define SLAUGHTERMASTER_H

#include <mawtribes/MawtribesBase.h>
#include <Weapon.h>

namespace OgorMawtribes
{

class Slaughtermaster : public MawtribesBase
{
public:

    static const int BASESIZE = 105; // x70 oval
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 140;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    Slaughtermaster();
    ~Slaughtermaster() override = default;

    bool configure();

protected:

    void onCastSpell(const Spell* spell, const Unit* target) override;
    void onUnboundSpell(const Unit* caster, int castRoll) override;
    void onStartHero(PlayerId player) override;

private:

    Weapon m_stumpBlades,
        m_bite,
        m_assortedWeapons;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Bloodgruel                       Yes
// Great Cauldron                   Yes
//    Bad Meat                      Yes
//    Troggoth Guts                 Yes
//    Spinemarrow                   Yes
//    Bonecrusher                   Yes
// Rockchomper                      TODO
//

} // namespace OgorMawtribes

#endif //SLAUGHTERMASTER_H