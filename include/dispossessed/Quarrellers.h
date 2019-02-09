/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef QUARRELLERS_H
#define QUARRELLERS_H

#include <Unit.h>
#include <Weapon.h>

namespace Dispossessed
{

class Quarrellers : public Unit
{
public:

    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 120;
    static const int POINTS_MAX_UNIT_SIZE = 360;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    Quarrellers();
    ~Quarrellers() override = default;

    bool configure(int numModels, bool duardinBucklers, bool standardBearer, bool drummer);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

private:

    bool m_duardinBucklers = false;
    bool m_standardBearer = false;
    bool m_drummer = false;

    Weapon m_duardinCrossbow,
        m_duardinCrossbowVeteran,
        m_rangersAxe;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Volley Fire                      No
// Ruinic Icon                      No
// Clan Banner                      No
// Duardin Bucklers                 No
//

} // namespace Dispossessed

#endif //QUARRELLERS_H