/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef MADCAPSHAMAN_H
#define MADCAPSHAMAN_H

#include <gloomspitegitz/GloomspiteGitzBase.h>
#include <Weapon.h>
#include <spells/GloomspiteSpells.h>

namespace GloomspiteGitz
{

class MadcapShaman : public GloomspiteGitzBase
{
public:

    static Unit* Create(const ParameterList& parameters);
    static void Init();
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);
    static int ComputePoints(int numModels);

    MadcapShaman();
    ~MadcapShaman() override = default;

    bool configure(LoreOfTheMoonclans lore);

protected:

private:

    Weapon m_moonStaff;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Madcap Mushroom                  TODO
// Night Shroud                     TODO
//

} // namespace GloomspiteGitz

#endif //MADCAPSHAMAN_H