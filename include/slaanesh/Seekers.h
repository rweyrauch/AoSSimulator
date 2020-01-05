/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SEEKERS_H
#define SEEKERS_H

#include <slaanesh/SlaaneshBase.h>
#include <Weapon.h>

namespace Slaanesh
{

class Seekers : public SlaaneshBase
{
public:

    static const int BASESIZE = 60; // x35 oval
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 120;
    static const int POINTS_MAX_UNIT_SIZE = 480;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    Seekers();
    ~Seekers() override = default;

    bool configure(int numModels, bool iconBearer, bool bannerBearer, bool hornblower);

protected:

    void computeBattleshockEffect(int roll, int& numFled, int& numAdded) const override;
    void restoreModels(int numModels) override;
    Rerolls chargeRerolls() const override;
    int runModifier() const override;

protected:

    bool m_iconBearer = false;
    bool m_bannerBearer = false;
    bool m_hornblower = false;

private:

    Weapon m_piercingClaws,
        m_piercingClawsHeartseeker,
        m_poisonedTongue;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Icon Bearers                     Yes
// Banner Bearers                   Yes
// Hornblowers                      No
// Quicksilver Speed                Yes
// Soul Hunters                     No

} // Slannesh

#endif //SEEKERS_H