/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ABHORRANTARCHREGENT_H
#define ABHORRANTARCHREGENT_H

#include <fec/FleshEaterCourts.h>
#include <Weapon.h>

namespace FleshEaterCourt
{

class AbhorrantArchregent : public FleshEaterCourts
{
public:
    static const int BASESIZE = 40;
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 240;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    AbhorrantArchregent();
    ~AbhorrantArchregent() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

    void onStartHero(PlayerId player) override;
    
private:

    Weapon m_goryTalonsAndFangs;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Imperial Blood                   Yes
// Ferocious Hunger                 No
// Summon Imperial Guard            No
//

} // namespace FleshEaterCourt

#endif //ABHORRANTARCHREGENT_H