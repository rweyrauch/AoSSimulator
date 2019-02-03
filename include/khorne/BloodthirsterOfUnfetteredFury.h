/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BLOODTHIRSTERFURY_H
#define BLOODTHIRSTERFURY_H

#include <Unit.h>
#include <Weapon.h>

namespace Khorne
{

class BloodthirsterOfUnfetteredFury : public Unit
{
public:
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 14;
    static const int POINTS_PER_UNIT = 260;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    BloodthirsterOfUnfetteredFury();
    ~BloodthirsterOfUnfetteredFury() override
    {
        delete m_pLashOfKhorne;
        delete m_pMightyAxeOfKhorne;
    }
    int move() const override;

    void hero(PlayerId player) override;

    bool configure();

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    int getDamageTableIndex() const;
    void onWounded() override;

private:

    Weapon* m_pLashOfKhorne = nullptr;
    Weapon* m_pMightyAxeOfKhorne = nullptr;

    static Weapon s_lashOfKhorne,
        s_mightyAxeOfKhorne;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Drawn in for the Kill            No
// The Land Rebels                  Yes*
// Rejoice in the Slaughter         No
//

} // namespace Khorne

#endif //BLOODTHIRSTERFURY_H