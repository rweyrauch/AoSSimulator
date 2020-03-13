/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef YLTHARI_H
#define YLTHARI_H

#include <sylvaneth/SylvanethBase.h>
#include <Weapon.h>

namespace Sylvaneth
{

class Ylthari : public SylvanethBase
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 180;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    Ylthari();
    ~Ylthari() override = default;

    bool configure();

protected:
    
private:

    Weapon m_briarStaff,
        m_thorns,
        m_snappingMandibles;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Vigour and Wrath                 TODO
// The Reaping                      TODO
//

} // namespace Sylvaneth

#endif //YLTHARI_H