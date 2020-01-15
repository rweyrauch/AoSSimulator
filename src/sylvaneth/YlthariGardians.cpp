#include <UnitFactory.h>
#include "sylvaneth/YlthariGardians.h"

namespace Sylvaneth
{
static FactoryMethod factoryMethod = {
    YltharisGuardians::Create,
    SylvanethBase::ValueToString,
    SylvanethBase::EnumStringToInt,
    YltharisGuardians::ComputePoints,
    {
    },
    ORDER,
    { SYLVANETH }
};

bool YltharisGuardians::s_registered = false;

Unit *YltharisGuardians::Create(const ParameterList &parameters)
{
    auto unit = new YltharisGuardians();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void YltharisGuardians::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Ylthari's Guardians", factoryMethod);
    }
}

YltharisGuardians::YltharisGuardians() :
    SylvanethBase("Ylthari's Guardians", 5, WOUNDS, 6, 5, false),
    m_enchantedGreatBlade(Weapon::Type::Melee, "Enchanted Greatblade", 1, 4, 3, 3, -1, 1),
    m_revenantBow(Weapon::Type::Missile, "Revenant Bow", 24, 2, 4, 3, -1, 1),
    m_protectorGlaive(Weapon::Type::Melee, "Protector Glaive", 1, 2, 4, 3, -1, 2),
    m_revenantBowMelee(Weapon::Type::Melee, "Revenent Bow (Melee)", 1, 1, 4, 3, 0, 1)
{
    m_keywords = {ORDER, SYLVANETH, OAKENBROW, TREE_REVENANTS, YLTHARIS_GUARDIANS};
    m_weapons = {&m_enchantedGreatBlade, &m_revenantBow, &m_protectorGlaive, &m_revenantBowMelee};
}

bool YltharisGuardians::configure()
{
    auto gallanghann = new Model(BASESIZE, WOUNDS);
    gallanghann->addMeleeWeapon(&m_protectorGlaive);
    gallanghann->setName("Gallanghann");
    addModel(gallanghann);

    auto skhathael = new Model(BASESIZE, WOUNDS);
    skhathael->addMeleeWeapon(&m_enchantedGreatBlade);
    skhathael->setName("Skhathael");
    addModel(skhathael);

    auto ahnslaine = new Model(BASESIZE, WOUNDS);
    ahnslaine->addMissileWeapon(&m_revenantBow);
    ahnslaine->addMeleeWeapon(&m_revenantBowMelee);
    ahnslaine->setName("Ahnslaine");
    addModel(ahnslaine);

    m_points = POINTS_PER_UNIT;

    return true;
}

Rerolls YltharisGuardians::toWoundRerolls(const Weapon *weapon, const Unit *target) const
{
    // Vigour and Wrath
    return RerollOnes;
}

} // namespace Sylvaneth