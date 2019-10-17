/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef FREECROSSBOWMEN_H
#define FREECROSSBOWMEN_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar
{

class FreeguildCrossbowmen : public CitizenOfSigmar
{
public:

    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 100;
    static const int POINTS_MAX_UNIT_SIZE = 300;

    static Unit* Create(const ParameterList& parameters);

    static std::string ValueToString(const Parameter &parameter);

    static int EnumStringToInt(const std::string &enumString);

    static void Init();

    FreeguildCrossbowmen();
    ~FreeguildCrossbowmen() override = default;

    bool configure(int numModels, bool standardBearer, bool piper);

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:


private:

    Weapon m_crossbow,
        m_dagger,
        m_crossbowMarksman;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Standard Bearer                  No
// Piper                            No
// Reload, Fire!                    No
//

} // namespace CitiesOfSigmar

#endif //FREECROSSBOWMEN_H