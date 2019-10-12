/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#ifndef SAVAGEBIGBOSS_H
#define SAVAGEBIGBOSS_H

#include "bonesplitterz/Bonesplitterz.h"

namespace Bonesplitterz
{
class SavageBigBoss : public Bonesplitterz
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 100;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    SavageBigBoss();
    ~SavageBigBoss() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:


private:

    Weapon m_chompasBoss;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Let Me at 'Em                    No
// Savage Attack                    No
//

} // namespace Bonesplitterz

#endif //SAVAGEBIGBOSS_H