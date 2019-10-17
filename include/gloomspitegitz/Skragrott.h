/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SKRAGROTT_H
#define SKRAGROTT_H

#include <gloomspitegitz/GloomspiteGitzBase.h>
#include <Weapon.h>
#include <spells/GloomspiteSpells.h>

namespace GloomspiteGitz
{

class Skragrott : public GloomspiteGitzBase
{
public:
    static const int BASESIZE = 60; // x35 oval
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 220;

    static Unit* Create(const ParameterList& parameters);
    static void Init();
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);

    Skragrott();
    ~Skragrott() override = default;

    bool configure(LoreOfTheMoonclans lore);

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

    int castingModifier() const override;
    int unbindingModifier() const override;
    Wounds applyWoundSave(const Wounds &wounds) override;

private:

    Weapon m_daMoonOnnaStikkMissile,
        m_daMoonOnnaStikk;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Babbling Wand                    No
// Da Moon Onna Stikk               No
// Loonking's Crown                 Yes
// Nikkit! Nikkit!                  Yes
// The Loonking's Entreaty          No
//

} // namespace GloomspiteGitz

#endif //SKRAGROTT_H