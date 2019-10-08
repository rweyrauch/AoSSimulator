/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ORRUKWARCHANTER_H
#define ORRUKWARCHANTER_H

#include <Unit.h>
#include <Weapon.h>

namespace Ironjawz
{

class OrrukWarchanter : public Unit
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 110;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    OrrukWarchanter();
    ~OrrukWarchanter() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

private:

    Weapon m_stikks;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Rhythm of Destruction            No
// Violent Fury                     No

} // namespace Ironjawz

#endif // ORRUKWARCHANTER_H