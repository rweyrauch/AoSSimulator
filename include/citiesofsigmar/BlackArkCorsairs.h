/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BLACKARKCORSAIRS_H
#define BLACKARKCORSAIRS_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar
{

class BlackArkCorsairs : public CitizenOfSigmar
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 40;
    static const int POINTS_PER_BLOCK = 80;
    static const int POINTS_MAX_UNIT_SIZE = 280;

    static Unit* Create(const ParameterList& parameters);

    static std::string ValueToString(const Parameter &parameter);

    static int EnumStringToInt(const std::string &enumString);

    static void Init();

    BlackArkCorsairs();
    ~BlackArkCorsairs() override = default;

    bool configure(int numModels, bool standardBearer, bool hornblower);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:


private:

    Weapon m_handbow,
        m_cutlass,
        m_blade,
        m_handbowReaver,
        m_cutlassReaver,
        m_bladeReaver;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Standard Bearer                  No
// Hornblower                       No
// Flashing Steel                   No
// Sea Dragon Cloak                 No
//

} // namespace CitiesOfSigmar

#endif //BLACKARKCORSAIRS_H