/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BLOODTHIRSTERFURY_H
#define BLOODTHIRSTERFURY_H

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne
{

class BloodthirsterOfUnfetteredFury : public KhorneBase
{
public:
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 14;
    static const int POINTS_PER_UNIT = 270;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    BloodthirsterOfUnfetteredFury();
    ~BloodthirsterOfUnfetteredFury() override = default;

    bool configure();

protected:

    int getDamageTableIndex() const;
    void onWounded() override;
    void onStartHero(PlayerId player) override;
    void onRestore() override;

private:

    Weapon m_lashOfKhorne,
        m_mightyAxeOfKhorne;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Drawn in for the Kill            TODO
// The Land Rebels                  Yes*
// Rejoice in the Slaughter         TODO
//

} // namespace Khorne

#endif //BLOODTHIRSTERFURY_H