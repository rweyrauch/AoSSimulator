/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BEASTSOFCHAOSBASE_H
#define BEASTSOFCHAOSBASE_H

#include <Unit.h>
#include <Weapon.h>

namespace BeastsOfChaos
{

class BeastsOfChaosBase : public Unit
{
public:
    enum Greatfray
    {
        None,
        Allherd,
        Darkwalkers,
        Gavespawn
    };

    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);

    BeastsOfChaosBase() = default;
    ~BeastsOfChaosBase() override = default;

    void setGreatfray(Greatfray fray);

protected:
    BeastsOfChaosBase(const std::string& name, int move, int wounds, int bravery, int save, bool fly) :
        Unit(name, move, wounds, bravery, save, fly) {}

protected:

    Greatfray m_greatfray = None;

};

//
// Abilities                    Implemented
// -------------------------------------------
// Brayherd Ambush                  TODO
// Creatures of the Storm           TODO
// Bloodgorge                       TODO
// Primordial Call                  TODO
// Bestial Might                    TODO
// Booming Roar                     TODO
// Dominator                        TODO
// Shadowbeasts                     TODO
// Nomadic Leader                   TODO
// Savage Encirclement              TODO
// Gift of Morghur                  TODO
// Propagator of Devolution         TODO
// Unravelling Aura                 TODO
//

void Init();

} // namespace BeastsOfChaos

#endif //BEASTSOFCHAOSBASE_H