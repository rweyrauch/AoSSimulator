/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef TUSKGORCHARIOTS_H
#define TUSKGORCHARIOTS_H

#include <beastsofchaos/BeastsOfChaosBase.h>
#include <Weapon.h>

namespace BeastsOfChaos
{

class TuskgorChariots : public BeastsOfChaosBase
{
public:

    static const int BASESIZE = 105; // x70 ovals
    static const int WOUNDS = 6;
    static const int MIN_UNIT_SIZE = 1;
    static const int MAX_UNIT_SIZE = 4;
    static const int POINTS_PER_BLOCK = 60;
    static const int POINTS_MAX_UNIT_SIZE = 240;

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static void Init();

    TuskgorChariots();
    ~TuskgorChariots() override = default;

    bool configure(int numModels);

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

    int toHitModifier(const Weapon* weapon, const Unit* unit) const override;
    Rerolls toHitRerolls(const Weapon* weapon, const Unit* unit) const override;
    int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;
    Rerolls chargeRerolls() const override;

private:

    Weapon m_despoilerAxe,
        m_gnarledSpear,
        m_tusksAndHooves;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Tuskgor Charge                   Yes
// Despoilers                       Yes
//

} // namespace BeastsOfChaos

#endif //TUSKGORCHARIOTS_H