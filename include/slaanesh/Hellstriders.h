/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef HELLSTRIDERS_H
#define HELLSTRIDERS_H

#include <slaanesh/SlaaneshBase.h>
#include <Weapon.h>

namespace Slaanesh
{

class Hellstriders : public SlaaneshBase
{
public:

    static const int BASESIZE = 60; // x35 oval
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 100;
    static const int POINTS_MAX_UNIT_SIZE = 400;

    enum WeaponOption
    {
        ClawSpear,
        Hellscourge
    };

    static Unit* Create(const ParameterList& parameters);
    static void Init();
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);

    Hellstriders();
    ~Hellstriders() override = default;

    bool configure(int numModels, WeaponOption weapons, bool iconBearer, bool bannerBearer, bool hornblower);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    Rerolls chargeRerolls() const override;
    int braveryModifier() const override;

protected:

    bool m_iconBearer = false;
    bool m_bannerBearer = false;
    bool m_hornblower = false;

private:

    Weapon m_clawSpear,
        m_clawSpearReaver,
        m_hellscourge,
        m_hellscourgeReaver,
        m_poisonedTongue;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Standard Banner                  Yes
// Icon Banner                      Yes
// Hornblower                       No
// Soul Hunters                     No
//
} // Slannesh

#endif //HELLSTRIDERS_H