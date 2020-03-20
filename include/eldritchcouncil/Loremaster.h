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

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    Loremaster();
    ~Loremaster() override = default;

    bool configure();

protected:

    Rerolls toSaveRerolls(const Weapon *weapon) const override;

private:

    Weapon m_greatsword;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Deflect Shots                    Yes
// Hand of Glory                    TODO
//

} // namespace EldritchCouncil

#endif //LOREMASTER_H