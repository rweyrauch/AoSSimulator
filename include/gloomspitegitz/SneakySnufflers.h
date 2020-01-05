/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SNEAKYSNUFFLERS_H
#define SNEAKYSNUFFLERS_H

#include <gloomspitegitz/GloomspiteGitzBase.h>
#include <Weapon.h>

namespace GloomspiteGitz
{

class SneakySnufflers : public GloomspiteGitzBase
{
public:
    static const int BASESIZE = 32;
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 6;
    static const int MAX_UNIT_SIZE = 18;
    static const int POINTS_PER_BLOCK = 70;
    static const int POINTS_MAX_UNIT_SIZE = 210;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    SneakySnufflers();
    ~SneakySnufflers() override = default;

    bool configure(int numModels);

protected:

private:

    Weapon m_sickle,
        m_gnashers;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Looncap Mushrooms                No
//

} // namespace GloomspiteGitz

#endif //SNEAKYSNUFFLERS_H