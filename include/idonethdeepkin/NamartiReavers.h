/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef NAMARTIREAVERS_H
#define NAMARTIREAVERS_H

#include <Unit.h>
#include <Weapon.h>

namespace IdonethDeepkin
{

class NamartiReavers : public Unit
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 140;
    static const int POINTS_MAX_UNIT_SIZE = 280;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    NamartiReavers();
    ~NamartiReavers() override
    {
        delete m_pWhisperbowAimedFire;
        delete m_pWhisperbowStormFire;
    }

    bool configure(int numModels, int numIconBearers);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

    void shooting(PlayerId player) override;

protected:

    // Swift Tide
    Rerolls runRerolls() const override { return RerollFailed; }

protected:

    int m_numIconBearers = 0;

    Weapon* m_pWhisperbowAimedFire = nullptr;
    Weapon* m_pWhisperbowStormFire = nullptr;

private:

    static Weapon s_keeningBlade,
        s_whisperbowAimedFire,
        s_whisperbowStormFire;

    static bool s_registered;
};

} // namespace IdonethDeepkin

#endif // NAMARTIREAVERSS_H
