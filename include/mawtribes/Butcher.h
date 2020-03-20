/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BUTCHER_H
#define BUTCHER_H

#include <mawtribes/MawtribesBase.h>
#include <Weapon.h>

namespace OgorMawtribes
{

class Butcher : public MawtribesBase
{
public:

    static Unit* Create(const ParameterList& parameters);
    static void Init();
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);
    static int ComputePoints(int numModels);

    enum WeaponOption
    {
        Tenderiser,
        Cleaver
    };

    Butcher();
    ~Butcher() override = default;

    bool configure(WeaponOption weaponOption);

protected:

    void onCastSpell(const Spell* spell, const Unit* target) override;
    void onUnboundSpell(const Unit* caster, int castRoll) override;

private:

    Weapon m_tenderizer,
        m_cleaver,
        m_bite;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Bloodgruel                       Yes
// Voracious Maw                    TODO
//

} // namespace OgorMawtribes

#endif //BUTCHER_H