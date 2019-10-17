/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LOREMASTER_H
#define LOREMASTER_H

#include <Unit.h>
#include <Weapon.h>

namespace EldritchCouncil
{

class Loremaster : public Unit
{
public:
    static const int BASESIZE = 25;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 140;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    Loremaster();
    ~Loremaster() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

    Rerolls toSaveRerolls(const Weapon *weapon) const override;

private:

    Weapon m_greatsword;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Deflect Shots                    Yes
// Hand of Glory                    No
//

} // namespace EldritchCouncil

#endif //LOREMASTER_H