/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef COS_WARDENKING_H
#define COS_WARDENKING_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar
{

class WardenKing : public CitizenOfSigmar
{
public:

    static const int BASESIZE = 25;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 110;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    WardenKing();
    ~WardenKing() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

    // Ancestor Shield
    Rerolls toSaveRerolls(const Weapon *weapon) const override { return RerollFailed; }

private:

    Weapon m_runeWeapon;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Ancestor Shield                  Yes
// Oath Stone                       No
// Ancestral Grudge                 No
//

} // namespace CitiesOfSigmar

#endif //COS_WARDENKING_H