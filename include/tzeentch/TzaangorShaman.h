/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef TZAANGORSHAMAN_H
#define TZAANGORSHAMAN_H

#include <Unit.h>
#include <Weapon.h>

namespace Tzeentch
{

class TzaangorShaman : public Unit
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 150;

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    TzaangorShaman();
    ~TzaangorShaman() override = default;

    bool configure();

protected:

private:

    Weapon m_staff,
        m_dagger,
        m_teethAndHorns;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Sorcerous Elixir                 No
// Visions of the Future            No
// Visions of the Past              No
// Boon of Mutation                 No
//

} // namespace Tzeentch

#endif //TZAANGORSHAMAN_H
