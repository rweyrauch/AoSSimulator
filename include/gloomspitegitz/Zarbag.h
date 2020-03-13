/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ZARBAG_H
#define ZARBAG_H

#include <gloomspitegitz/GloomspiteGitzBase.h>
#include <Weapon.h>
#include <spells/GloomspiteSpells.h>

namespace GloomspiteGitz
{

class Zarbag : public GloomspiteGitzBase
{
public:
    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 80;

    static Unit* Create(const ParameterList& parameters);
    static void Init();
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };

    Zarbag();
    ~Zarbag() override = default;

    bool configure(LoreOfTheMoonclans lore);

protected:

private:

    Weapon m_sickle;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Sniffer Spite                    TODO
// Face of Da Bad Moon              TODO
//

} // namespace GloomspiteGitz

#endif //ZARBAG_H