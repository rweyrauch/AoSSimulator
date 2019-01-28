/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef GHORGON_H
#define GHORGON_H

#include <Unit.h>
#include <Weapon.h>

namespace BeastsOfChaos
{

class Ghorgon : public Unit
{
public:

    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 14;
    static const int POINTS_PER_UNIT = 200;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    Ghorgon();
    ~Ghorgon() override
    {
        delete m_pButcheringBlades;
        delete m_pHugeSlaveringMaw;
    }

    bool configure();
    int move() const override;

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    void onWounded() override;
    int getDamageTableIndex() const;
    int generateMortalWounds(const Weapon* weapon, const Unit* unit, const Hits& hits) const override;
    void onStartCombat(PlayerId player) override;

private:

    Weapon* m_pButcheringBlades = nullptr;
    Weapon* m_pHugeSlaveringMaw = nullptr;

    static Weapon s_butcheringBlades,
        s_hugeSlaveringMaw;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Ravenous Bloodgreed              Yes
// Swallow Whole                    Yes
//

} // namespace BeastsOfChaos

#endif //GHORGON_H