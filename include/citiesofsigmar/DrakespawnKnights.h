/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef DRAKESPAWNKNIGHTS_H
#define DRAKESPAWNKNIGHTS_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar
{

class DrakespawnKnights : public CitizenOfSigmar
{
public:

    static const int BASESIZE = 60;
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 150;
    static const int POINTS_MAX_UNIT_SIZE = 150 * 4;

    static Unit* Create(const ParameterList& parameters);

    static std::string ValueToString(const Parameter &parameter);

    static int EnumStringToInt(const std::string &enumString);

    static void Init();

    DrakespawnKnights();
    ~DrakespawnKnights() override = default;

    bool configure(int numModels, bool standardBearer, bool hornblower);

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

    int runModifier() const override;
    int chargeModifier() const override;
    int braveryModifier() const override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    int weaponRend(const Weapon* weapon, const Unit* target, int hitRoll, int woundRoll) const override;

private:

    bool m_standardBearer = false;
    bool m_hornblower = false;

    Weapon m_lance,
        m_lanceDreadKnight,
        m_jaws;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Standard Bearer                  Yes
// Hornblower                       Yes
// Lance Charge                     Yes
//

} // namespace CitiesOfSigmar

#endif //DRAKESPAWNKNIGHTS_H