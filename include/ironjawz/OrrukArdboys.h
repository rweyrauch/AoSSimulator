/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ORRUKARKBOYS_H
#define ORRUKARKBOYS_H

#include <ironjawz/Ironjawz.h>
#include <Weapon.h>

namespace Ironjawz
{

class OrrukArdboys : public Ironjawz
{
public:
    static const int BASESIZE = 32;
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 90;
    static const int POINTS_MAX_UNIT_SIZE = 540;


    enum StandardOption
    {
        None = 0,
        BannerBearer,
        GlyphBearer
    };

    static Unit* Create(const ParameterList& parameters);

    static std::string ValueToString(const Parameter& parameter);

    static int EnumStringToInt(const std::string& enumString);

    static void Init();

    OrrukArdboys();
    ~OrrukArdboys() override = default;

    bool configure(int numModels, int numShields, bool drummer, StandardOption standard);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

    int bravery() const override;

protected:

    void computeBattleshockEffect(int roll, int& numFled, int& numAdded) const override;
    int chargeModifier() const override;
    Wounds applyWoundSave(const Wounds &wounds) override;

protected:

    Weapon m_choppa,
        m_bossChoppa;

    bool m_drummer = false;
    StandardOption m_standardBearer = None;
    int m_numShields = 0;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Drummer                          Yes
// Banner                           Yes
// Glyph Bearer                     No
// Orruk-forged Shields             Yes
//

} // namespace Ironjawz

#endif // ORRUKARKBOYS_H