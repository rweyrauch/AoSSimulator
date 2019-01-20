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

    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 30;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    WitchAelves();
    ~WitchAelves() override = default;

    bool configure(int numModels, bool pairedKnives, bool hornblowers, bool standardBearers);

protected:

    int extraAttacks(const Weapon* weapon) const override;
    int rollBattleshock() const override;
    int toSaveModifier(const Weapon* weapon) const override;

protected:

    bool m_pairedKnives = true;
    bool m_hornblowers = false;
    bool m_standardBearers = false;

private:

    static Weapon s_sacrificialKnife,
        s_sacrificialKnifeHag;

    static bool s_registered;
};

} // namespace DaughtersOfKhaine

#endif //WITCHAELVES_H