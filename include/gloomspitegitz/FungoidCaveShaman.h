/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CAVESHAMAN_H
#define CAVESHAMAN_H

#include <gloomspitegitz/GloomspiteGitzBase.h>
#include <Weapon.h>
#include <spells/GloomspiteSpells.h>

namespace GloomspiteGitz
{

class FungoidCaveShaman : public GloomspiteGitzBase
{
public:
    static const int BASESIZE = 32;
    static const int WOUNDS = 4;
    static const int POINTS_PER_UNIT = 90;

    static Unit* Create(const ParameterList& parameters);
    static void Init();
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };

    FungoidCaveShaman();
    ~FungoidCaveShaman() override = default;

    bool configure(LoreOfTheMoonclans lore);

protected:

    void onStartHero(PlayerId playerId) override;

private:

    Weapon m_moonSickle,
        m_squigsTeeth;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Mouthpiece of Mork               TODO
// Deffcap Mushroom                 TODO
// Spore Squig                      TODO
// Spore Maws                       TODO
//

} // namespace GloomspiteGitz

#endif //CAVESHAMAN_H