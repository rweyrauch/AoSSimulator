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

    FungoidCaveShaman();
    ~FungoidCaveShaman() override = default;

    bool configure(LoreOfTheMoonclans lore);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

private:

    Weapon m_moonSickle,
        m_squigsTeeth;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Mouthpiece of Mork               No
// Deffcap Mushroom                 No
// Spore Squig                      No
// Spore Maws                       No
//

} // namespace GloomspiteGitz

#endif //CAVESHAMAN_H