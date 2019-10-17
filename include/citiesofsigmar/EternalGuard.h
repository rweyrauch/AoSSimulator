/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef COS_ETERNALGUARD_H
#define COS_ETERNALGUARD_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar
{

class EternalGuard : public CitizenOfSigmar
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 130;
    static const int POINTS_MAX_UNIT_SIZE = 330;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    EternalGuard();
    ~EternalGuard() override = default;

    bool configure(int numModels, bool standardBearer, bool hornblower, bool gladeShields);

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

    void onStartHero(PlayerId player) override;
    Rerolls runRerolls() const override;
    int braveryModifier() const override;
    Rerolls toSaveRerolls(const Weapon *weapon) const override;
    int toHitModifier(const Weapon *weapon, const Unit *target) const override { return m_fortressModifier; }
    int toWoundModifier(const Weapon *weapon, const Unit *target) const override { return m_fortressModifier; }
    int toSaveModifier(const Weapon *weapon) const override { return m_fortressModifier; }

private:

    bool m_hornblower = false,
        m_standardBearer = false,
        m_gladeShields = false;

    int m_fortressModifier = 0;

    Weapon m_spearStave,
        m_spearStaveWarden;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Fortress of Boughs               Yes
// Glade Shields                    Yes
// Standard Bearer                  Yes
// Hornblower                       Yes
//

} // namespace CitiesOfSigmar

#endif //COS_ETERNALGUARD_H