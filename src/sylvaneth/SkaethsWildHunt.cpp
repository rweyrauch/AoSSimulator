#include <UnitFactory.h>
#include "sylvaneth/SkaethsWildHunt.h"

namespace Sylvaneth
{
static FactoryMethod factoryMethod = {
    SkaethsWildHunt::Create,
    SylvanethBase::ValueToString,
    SylvanethBase::EnumStringToInt,
    {
    },
    ORDER,
    SYLVANETH
};

bool SkaethsWildHunt::s_registered = false;


Unit *SkaethsWildHunt::Create(const ParameterList &parameters)
{
    auto unit = new SkaethsWildHunt();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void SkaethsWildHunt::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Skaeth's Wild Hunt", factoryMethod);
    }
}

SkaethsWildHunt::SkaethsWildHunt() :
    SylvanethBase("Skaeth's Wild Hunt", 6, WOUNDS, 6, 5, false),
    m_seekerBow(Weapon::Type::Missile, "Seeker Bow", 18,1, 3, 4, -1, 1),
    m_javalin(Weapon::Type::Missile, "Javalin of the Hunt", 9, 1, 3, 3, -1, 2),
    m_javalinMelee(Weapon::Type::Melee, "Javalin of the Hunt (Melee)", 2, 1, 3, 3, -1, 2),
    m_huntingWeapon(Weapon::Type::Melee, "Hunting Weapon", 1, 2, 3, 4, 0, 1),
    m_teethAndClaws(Weapon::Type::Melee, "Teeth and Claws", 1, 2, 3, 3, 0, 1)
{
    m_keywords = {ORDER, SYLVANETH, OAKENBROW, SKAETHS_WILD_HUNT};

    // Fleet of Foot
    m_runAndCharge = true;
    m_runAndShoot = true;

    m_totalUnbinds = 1;
    m_totalSpells = 1;
}

bool SkaethsWildHunt::configure()
{
    Model skaeth(BASESIZE, WOUNDS);
    skaeth.addMissileWeapon(&m_javalin);
    skaeth.addMeleeWeapon(&m_javalinMelee);
    skaeth.setName("Skaeth");
    addModel(skaeth);

    Model althaen(BASESIZE, WOUNDS);
    althaen.addMissileWeapon(&m_seekerBow);
    althaen.addMeleeWeapon(&m_huntingWeapon);
    althaen.setName("Althaen");
    addModel(althaen);

    Model karthaen(BASESIZE, WOUNDS);
    karthaen.addMeleeWeapon(&m_huntingWeapon);
    karthaen.setName("Karthaen");
    addModel(karthaen);

    Model sheoch(BASESIZE, WOUNDS);
    sheoch.addMeleeWeapon(&m_huntingWeapon);
    sheoch.setName("Sheoch");
    addModel(sheoch);

    Model lighaen(BASESIZE, WOUNDS);
    lighaen.addMeleeWeapon(&m_teethAndClaws);
    lighaen.setName("Lighaen");
    addModel(lighaen);

    m_points = POINTS_PER_UNIT;

    return true;
}

void SkaethsWildHunt::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_seekerBow);
    visitor(&m_javalin);
    visitor(&m_javalinMelee);
    visitor(&m_huntingWeapon);
    visitor(&m_teethAndClaws);
}

} // namespace Sylvaneth