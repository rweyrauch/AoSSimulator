/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef DOOMFLAYER_H
#define DOOMFLAYER_H

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven
{

class Doomflayer : public Skaventide
{
public:
    static const int BASESIZE = 60; // x35 oval
    static const int WOUNDS = 3;
    static const int POINTS_PER_UNIT = 60;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    Doomflayer();
    ~Doomflayer() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

    int toHitModifier(const Weapon *weapon, const Unit *target) const override;

private:

    Weapon m_whirlingBlades,
        m_rustyKnives;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Whirling Death                   Yes
// More-more Whirling Death!        No
//

} // namespace Skaven

#endif //DOOMFLAYER_H