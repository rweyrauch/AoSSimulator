/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef MORATHIQUEEN_H
#define MORATHIQUEEN_H

#include <dok/DaughterOfKhaine.h>
#include <Weapon.h>

namespace DaughtersOfKhaine
{

class MorathiTheShadowQueen : public DaughterOfKhaine
{
public:

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    MorathiTheShadowQueen();
    ~MorathiTheShadowQueen() override = default;

    bool configure();

protected:

    void onWounded() override;
    void onRestore() override;

private:

    int getDamageTableIndex() const;

    Weapon m_gaze,
        m_heartrender,
        m_crown,
        m_tail;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Monsterous Revelation            TODO
// Gaze of Morathi                  TODO
// The Iron Heart of Khaine         TODO
// Arnzipal's Black Horror          TODO
//

} // namespace DaughtersOfKhaine

#endif //MORATHIQUEEN_H