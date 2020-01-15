/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef GLADEGUARD_H
#define GLADEGUARD_H

#include <wanderers/Wanderer.h>
#include <Weapon.h>

namespace Wanderers
{

class GladeGuard : public Wanderer
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 120;
    static const int POINTS_MAX_UNIT_SIZE = 360;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    GladeGuard();
    ~GladeGuard() override = default;

    bool configure(int numModels, bool pennantBearer, bool hornblower);

protected:

    int toHitModifier(const Weapon *weapon, const Unit *target) const override;
    Rerolls runRerolls() const override;
    int braveryModifier() const override;

    void onStartShooting(PlayerId player) override;

private:

    bool m_hornblower = false,
        m_pennantBearer = false;

    bool m_usedArcaneBodkins = false;

    Weapon m_longbow,
        m_longbowLord,
        m_gladeBlade;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Peerless Archery                 Yes
// Ancane Bodkins                   Yes
// Pennant Bearer                   Yes
// Hornblower                       Yes
//

} // namespace Wanderers

#endif //GLADEGUARD_H