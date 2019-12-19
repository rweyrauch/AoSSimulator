/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef AURICRUNESON_H
#define AURICRUNESON_H

#include <fyreslayers/Fyreslayer.h>
#include <Weapon.h>

namespace Fyreslayers
{

class AuricRuneson : public Fyreslayer
{
public:
    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 100;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    AuricRuneson();
    ~AuricRuneson() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

private:

    Weapon m_throwingAxe,
        m_javelin,
        m_warAxe,
        m_javelinMelee;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Vying for Glory                  No
// Wyrmslayer Javelins              No
// Dauntless Assault                No
//

} // namespace Fyreslayers

#endif //AURICRUNESON_H