/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BLADEBRINGERHELLFLAYER_H
#define BLADEBRINGERHELLFLAYER_H

#include <slaanesh/SlaaneshBase.h>
#include <Weapon.h>

namespace Slaanesh
{

class BladebringerOnHellflayer : public SlaaneshBase
{
public:

    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 9;
    static const int POINTS_PER_UNIT = 180;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    BladebringerOnHellflayer();
    ~BladebringerOnHellflayer() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

private:

    Weapon m_flensingWhips,
        m_piercingClaws,
        m_poisonedTongues;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Soulscent                        No
// Acquiescence                     No
//

} // Slannesh

#endif //BLADEBRINGERHELLFLAYER_H