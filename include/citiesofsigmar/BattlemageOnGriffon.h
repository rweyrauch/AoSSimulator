/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BATTLEMAGEGRIFFON_H
#define BATTLEMAGEGRIFFON_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar
{

class BattlemageOnGriffon : public CitizenOfSigmar
{
public:

    static const int BASESIZE = 90;
    static const int WOUNDS = 13;
    static const int POINTS_PER_UNIT = 300;

    static Unit* Create(const ParameterList& parameters);

    static std::string ValueToString(const Parameter &parameter);

    static int EnumStringToInt(const std::string &enumString);

    static void Init();

    BattlemageOnGriffon();
    ~BattlemageOnGriffon() override = default;

    int move() const override;

    bool configure();

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

    void onWounded() override;
    int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

private:

    int getDamageTableIndex() const;

    Weapon m_beastStaff,
        m_twinBeaks,
        m_razorClaws;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Amber Battlemage                 No
// Two-headed                       Yes
// Amber Spear                      No
// Wildform                         No
//

} // namespace CitiesOfSigmar

#endif //BATTLEMAGEGRIFFON_H