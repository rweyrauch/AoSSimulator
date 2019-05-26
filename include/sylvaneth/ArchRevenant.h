/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ARCHREVENANT_H
#define ARCHREVENANT_H

#include <Unit.h>
#include <Weapon.h>

namespace Sylvaneth
{

class ArchRevenant : public Unit
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 100;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    ArchRevenant();
    ~ArchRevenant() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

private:

    Weapon m_glaive,
        m_tailPincers;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Crescent Shield                  No
// Champion of Kurnoth              No
// Ultimate Sacrifice               No
// Call to Battle                   No
//

} // namespace Sylvaneth

#endif //ARCHREVENANT_H