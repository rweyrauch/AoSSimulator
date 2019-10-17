/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LOONBOSSWITHCAVESQUIG_H
#define LOONBOSSWITHCAVESQUIG_H

#include <gloomspitegitz/GloomspiteGitzBase.h>
#include <Weapon.h>

namespace GloomspiteGitz
{

class LoonbossWithGiantCaveSquig : public GloomspiteGitzBase
{
public:
    static const int BASESIZE = 50;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 100;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    LoonbossWithGiantCaveSquig();
    ~LoonbossWithGiantCaveSquig() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

    int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;

private:

    Weapon m_moonProdderMissile,
        m_moonProdder,
        m_massiveFangFilledGob;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Dead Tricksy                     Yes
// I'm Da Boss, Now Stab 'Em Good   No
//

} // namespace GloomspiteGitz

#endif //LOONBOSSONCAVESQUIG_H