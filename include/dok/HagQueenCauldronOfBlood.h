/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef HAGQUEENCAULDRON_H
#define HAGQUEENCAULDRON_H

#include <dok/DaughterOfKhaine.h>
#include <Weapon.h>

namespace DaughtersOfKhaine
{

class HagQueenOnCauldronOfBlood : public DaughterOfKhaine
{
public:

    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 13;
    static const int POINTS_PER_UNIT = 0;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    HagQueenOnCauldronOfBlood();
    ~HagQueenOnCauldronOfBlood() override = default;

    bool configure();

protected:

    void onWounded() override;
    void onRestore() override;

private:

    int getDamageTableIndex() const;

    Weapon m_burningBlood,
        m_knives,
        m_blade,
        m_sword;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Bladed Impact                    TODO
// Bloodshield                      TODO
// Witchbrew                        TODO
// Priestess of Khaine              TODO
// Wrath of Khaine                  TODO
// Animated                         TODO
// Idol of Worship                  TODO
//

} // namespace DaughtersOfKhaine

#endif //HAGQUEENCAULDRON_H