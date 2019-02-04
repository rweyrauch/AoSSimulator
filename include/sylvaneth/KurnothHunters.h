/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef KURNOTHHUNTERS_H
#define KURNOTHHUNTERS_H

#include <Unit.h>
#include <Weapon.h>

namespace Sylvaneth
{

class KurnothHunters : public Unit
{
public:

    static const int BASESIZE = 50;
    static const int WOUNDS = 5;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 200;
    static const int POINTS_MAX_UNIT_SIZE = 800;

    enum WeaponOption
    {
        Greatswords,
        Scythes,
        Greatbows
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static void Init();

    KurnothHunters();
    ~KurnothHunters() override = default;

    bool configure(int numModels, WeaponOption weapons);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    WeaponOption m_weaponOption = Greatswords;

private:

    Weapon m_greatbow,
        m_greatbowHuntmaster,
        m_greatsword,
        m_greatswordHuntmaster,
        m_scythe,
        m_scytheHuntmaster,
        m_viciousClaws;

    static bool s_registered;
};

} // namespace Sylvaneth

#endif //KURNOTHHUNTERS_H