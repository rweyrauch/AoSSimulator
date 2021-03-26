/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "sylvaneth/YlthariGardians.h"

namespace Sylvaneth {
    static const int g_basesize = 32;
    static const int g_wounds = 1;
    static const int g_pointsPerUnit = 0;

    bool YltharisGuardians::s_registered = false;

    Unit *YltharisGuardians::Create(const ParameterList &parameters) {
        auto unit = new YltharisGuardians();

        unit->configure();
        return unit;
    }

    void YltharisGuardians::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    YltharisGuardians::Create,
                    SylvanethBase::ValueToString,
                    SylvanethBase::EnumStringToInt,
                    YltharisGuardians::ComputePoints,
                    {
                    },
                    ORDER,
                    {SYLVANETH}
            };
            s_registered = UnitFactory::Register("Ylthari's Guardians", factoryMethod);
        }
    }

    YltharisGuardians::YltharisGuardians() :
            SylvanethBase("Ylthari's Guardians", 5, g_wounds, 6, 5, false),
            m_enchantedGreatBlade(Weapon::Type::Melee, "Enchanted Greatblade", 1, 4, 3, 3, -1, 1),
            m_revenantBow(Weapon::Type::Missile, "Revenant Bow", 24, 2, 4, 3, -1, 1),
            m_protectorGlaive(Weapon::Type::Melee, "Protector Glaive", 1, 2, 4, 3, -1, 2),
            m_revenantBowMelee(Weapon::Type::Melee, "Revenent Bow (Melee)", 1, 1, 4, 3, 0, 1) {
        m_keywords = {ORDER, SYLVANETH, OAKENBROW, TREE_REVENANTS, YLTHARIS_GUARDIANS};
        m_weapons = {&m_enchantedGreatBlade, &m_revenantBow, &m_protectorGlaive, &m_revenantBowMelee};
    }

    bool YltharisGuardians::configure() {
        auto gallanghann = new Model(g_basesize, wounds());
        gallanghann->addMeleeWeapon(&m_protectorGlaive);
        gallanghann->setName("Gallanghann");
        addModel(gallanghann);

        auto skhathael = new Model(g_basesize, wounds());
        skhathael->addMeleeWeapon(&m_enchantedGreatBlade);
        skhathael->setName("Skhathael");
        addModel(skhathael);

        auto ahnslaine = new Model(g_basesize, wounds());
        ahnslaine->addMissileWeapon(&m_revenantBow);
        ahnslaine->addMeleeWeapon(&m_revenantBowMelee);
        ahnslaine->setName("Ahnslaine");
        addModel(ahnslaine);

        m_points = g_pointsPerUnit;

        return true;
    }

    Rerolls YltharisGuardians::toWoundRerolls(const Weapon * /*weapon*/, const Unit * /*target*/) const {
        // Vigour and Wrath
        return Rerolls::Ones;
    }

    int YltharisGuardians::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace Sylvaneth