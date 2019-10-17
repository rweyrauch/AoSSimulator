/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SQUIGGOBBA_H
#define SQUIGGOBBA_H

#include <gloomspitegitz/GloomspiteGitzBase.h>
#include <Weapon.h>

namespace GloomspiteGitz
{

class SquigGobba : public GloomspiteGitzBase
{
public:
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 8;
    static const int POINTS_PER_UNIT = 160;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    SquigGobba();
    ~SquigGobba() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

    int toHitModifier(const Weapon *weapon, const Unit *target) const override;

private:

    Weapon m_spitSquigs,
        m_bashinSticks,
        m_cavernousMaw;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Arcing Spit                      Yes
//

} // namespace GloomspiteGitz

#endif //SQUIGGOBBA_H