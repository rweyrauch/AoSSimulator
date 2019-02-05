/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef MANGLERSQUIGS_H
#define MANGLERSQUIGS_H

#include <Unit.h>
#include <Weapon.h>

namespace GloomspiteGitz
{

class ManglerSquigs : public Unit
{
public:
    static const int BASESIZE = 80;
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 240;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    ManglerSquigs();
    ~ManglerSquigs() override = default;

    int move() const override;

    bool configure();

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    int toHitModifier(const Weapon* weapon, const Unit* unit) const override;
    void onWounded() override;
    void onSlain() override;

    int getDamageTableIndex() const;

private:

    Weapon m_hugeFangFilledGob,
        m_ballsAndChains,
        m_grotsBashinStikk;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Ker-splat!                       Yes
// Watch Out!                       Yes
//

} // namespace GloomspiteGitz

#endif //MANGLERSQUIGS_H