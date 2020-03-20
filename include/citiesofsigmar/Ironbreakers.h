/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef COS_IRONBREAKERS_H
#define COS_IRONBREAKERS_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar
{

class Ironbreakers : public CitizenOfSigmar
{
public:

    enum WeaponOptions
    {
        IronbreakerAxeOrHammer,
        DrakefirePistolAndCinderblastBomb,
        PairedDrakefirePistols
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);
    static int ComputePoints(int numModels);
    static void Init();

    Ironbreakers();
    ~Ironbreakers() override = default;

    bool configure(int numModels, WeaponOptions ironbeardWeapons, bool standardBearer, bool drummer);

protected:

    Rerolls toSaveRerolls(const Weapon *weapon) const override;
    int toSaveModifier(const Weapon *weapon) const override;
    void onStartShooting(PlayerId player) override;
    int runModifier() const override;
    int chargeModifier() const override;
    int braveryModifier() const override;

private:

    bool m_standardBearer = false;
    bool m_drummer = false;

    bool m_hasCinderblastBomb = false;

    Weapon m_drakefirePistol,
        m_drakefirePistolMelee,
        m_axeOrHammer,
        m_axeOrHammerIronbeard;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Brace of Drakefire Pistols       Yes
// Cinderblast Bomb                 Yes
// Gromril Shields                  Yes
// Forge-proven Gromril Armour      Yes
// Musician                         Yes
// Standard Bearer                  Yes
//

} // namespace CitiesOfSigmar

#endif //COS_IRONBREAKERS_H