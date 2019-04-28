/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef VICELEADER_H
#define VICELEADER_H

#include <Unit.h>
#include <Weapon.h>

namespace Slaanesh
{

class ViceleaderHeraldOfSlaanesh : public Unit
{
public:

    static const int BASESIZE = 25;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 120;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    ViceleaderHeraldOfSlaanesh();
    ~ViceleaderHeraldOfSlaanesh() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

private:

    Weapon m_ravagingClaws;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Lightning Reflexes               No
// Lithe and Swift                  Yes
// Acquiescence                     No
//

} // Slannesh

#endif //VICELEADER_H