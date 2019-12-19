/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef WITCHAELVES_H
#define WITCHAELVES_H

#include <dok/DaughterOfKhaine.h>
#include <Weapon.h>

namespace DaughtersOfKhaine
{

class WitchAelves : public DaughterOfKhaine
{
public:

    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 120;
    static const int POINTS_MAX_UNIT_SIZE = 300;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    WitchAelves();
    ~WitchAelves() override = default;

    bool configure(int numModels, bool pairedKnives, bool hornblowers, bool standardBearers);

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

    int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;
    int rollBattleshock() const override;
    int toSaveModifier(const Weapon* weapon) const override;
    Wounds computeReturnedDamage(const Weapon* weapon, int saveRoll) const override;

protected:

    bool m_pairedKnives = true;
    bool m_hornblowers = false;
    bool m_standardBearers = false;

private:

    Weapon m_sacrificialKnife,
        m_sacrificialKnifeHag;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Paired Sacrificial Knives        Yes
// Frenzied Fervour                 No
// Bladed Bucklers                  Yes
//

} // namespace DaughtersOfKhaine

#endif //WITCHAELVES_H