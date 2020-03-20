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

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
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