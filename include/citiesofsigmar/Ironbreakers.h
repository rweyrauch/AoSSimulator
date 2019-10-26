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

    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 130;
    static const int POINTS_MAX_UNIT_SIZE = 330;

    enum WeaponOptions
    {
        IronbreakerAxeOrHammer,
        DrakefirePistolAndCinderblastBomb,
        PairedDrakefirePistols
    };

    static Unit* Create(const ParameterList& parameters);

    static std::string ValueToString(const Parameter &parameter);

    static int EnumStringToInt(const std::string &enumString);

    static void Init();

    Ironbreakers();
    ~Ironbreakers() override = default;

    bool configure(int numModels, WeaponOptions ironbeardWeapons, bool standardBearer, bool drummer);

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

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
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Brace of Drakefire Pistols       Yes
// Cinderblast Bomb                 Yes
// Gromril Shields                  Yes
// Forge-proven Gromril Armour      Yes
// Musician                         Yes
// Icon Bearer                      No
//

} // namespace CitiesOfSigmar

#endif //COS_IRONBREAKERS_H