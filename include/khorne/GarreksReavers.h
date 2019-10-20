/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef GARREKSREAVERS_H
#define GARREKSREAVERS_H

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne
{

class GarreksReavers : public KhorneBase
{
public:
    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int POINTS_PER_UNIT = 40;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    GarreksReavers();
    ~GarreksReavers() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:


private:

    Weapon m_garreksAxe,
        m_KarusAxe,
        m_SaeksAxe,
        m_blades;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Gorebeard                        No
// Frenzied Devotion                No
// Reaver Blades                    No
// Grisly Trophies                  No
//

} // namespace Khorne

#endif //GARREKSREAVERS_H