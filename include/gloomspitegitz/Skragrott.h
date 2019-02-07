/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SKRAGROTT_H
#define SKRAGROTT_H

#include <Unit.h>
#include <Weapon.h>

namespace GloomspiteGitz
{

class Skragrott : public Unit
{
public:
    static const int BASESIZE = 60; // x35 oval
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 220;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    Skragrott();
    ~Skragrott() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

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
// Loonking's Crown                 No
// Nikkit! Nikkit!                  No
// The Loonking's Entreaty          No
//

} // namespace GloomspiteGitz

#endif //SKRAGROTT_H