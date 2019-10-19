/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef DEMIGRYPHKNIGHTS_H
#define DEMIGRYPHKNIGHTS_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar
{

class DemigryphKnights : public CitizenOfSigmar
{
public:

    static const int BASESIZE = 65;
    static const int WOUNDS = 4;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 180;
    static const int POINTS_MAX_UNIT_SIZE = POINTS_PER_BLOCK * 4;

    enum WeaponOption
    {
        Halberd,
        Lance
    };

    static Unit* Create(const ParameterList& parameters);

    static std::string ValueToString(const Parameter &parameter);

    static int EnumStringToInt(const std::string &enumString);

    static void Init();

    DemigryphKnights();
    ~DemigryphKnights() override = default;

    bool configure(int numModels, bool standardBearer, bool hornblower, WeaponOption weapons);

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:


private:

    bool m_standardBearer = false;
    bool m_hornblower = false;

    Weapon m_halberd,
        m_lance,
        m_halberdPreceptor,
        m_lancePreceptor,
        m_beakAndTalons;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Standard Bearer                  No
// Hornblower                       No
// Charging Lance                   No
// Savage Ferocity                  No
//

} // namespace CitiesOfSigmar

#endif //DEMIGRYPHKNIGHTS_H