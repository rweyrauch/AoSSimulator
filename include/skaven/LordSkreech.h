/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LORDSKREECHVERMINKING_H
#define LORDSKREECHVERMINKING_H

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven
{

class LordSkreechVerminking : public Skaventide
{
public:
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 320;

    static Unit *Create(const ParameterList &parameters);

    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };

    static void Init();

    LordSkreechVerminking();

    ~LordSkreechVerminking() override;

    bool configure();

protected:

    Wounds applyWoundSave(const Wounds &wounds) override;
    void onWounded() override;
    void onRestore() override;

    int terrifying(const Unit* target);
    int getDamageTableIndex() const;

private:

    Weapon m_tails,
        m_glaive,
        m_plaguereaper;

    lsignal::slot m_connection;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Protection of the Horned Rat     Yes
// Terrifying                       Yes
// The Thirteen-headed One          TODO
// THe Rat King                     TODO
//

} // namespace Skaven

#endif //LORDSKREECHVERMINKING_H