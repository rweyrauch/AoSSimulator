/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SKULLCRUSHERS_H
#define SKULLCRUSHERS_H

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne
{

class MightySkullcrushers : public KhorneBase
{
public:
    static const int BASESIZE = 90; // x52 oval
    static const int WOUNDS = 5;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 160;
    static const int POINTS_MAX_UNIT_SIZE = 160*4;

    enum WeaponOption
    {
        EnsorcelledAxe,
        Bloodglaive,
    };

    static Unit* Create(const ParameterList& parameters);
    static void Init();
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static int ComputePoints(int numModels);

    MightySkullcrushers();
    ~MightySkullcrushers() override = default;

    bool configure(int numModels, WeaponOption weapons, bool standardBearer, bool hornblowers);

protected:

    int chargeModifier() const override;
    int runModifier() const override;
    int braveryModifier() const override;
    void onCharged() override;

private:

    WeaponOption m_weaponOption = EnsorcelledAxe;
    bool m_standarBearer = false;
    bool m_hornblower = false;

    Weapon m_ensorcelledAxe,
        m_bloodglaive,
        m_ensorcelledAxeHunter,
        m_bloodglaiveHunter,
        m_brazenHooves;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Hornblower                       Yes
// Standard Bearer                  Yes
// Murderous Charge                 Yes
//

} // namespace Khorne

#endif //SKULLCRUSHERS_H