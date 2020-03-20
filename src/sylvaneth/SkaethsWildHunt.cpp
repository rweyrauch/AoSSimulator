#include <UnitFactory.h>
#include "sylvaneth/SkaethsWildHunt.h"

namespace Sylvaneth
{
static const int BASESIZE = 32;
static const int WOUNDS = 1;
static const int POINTS_PER_UNIT = 120;

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
        static FactoryMethod factoryMethod = {
            SkaethsWildHunt::Create,
            SylvanethBase::ValueToString,
            SylvanethBase::EnumStringToInt,
            SkaethsWildHunt::ComputePoints,
            {
            },
            ORDER,
            { SYLVANETH }
        };
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
    m_weapons = {&m_seekerBow, &m_javalin, &m_javalinMelee, &m_huntingWeapon, &m_teethAndClaws};

    // Fleet of Foot
    m_runAndCharge = true;
    m_runAndShoot = true;

    m_totalUnbinds = 1;
    m_totalSpells = 1;
}

bool SkaethsWildHunt::configure()
{
    auto skaeth = new Model(BASESIZE, wounds());
    skaeth->addMissileWeapon(&m_javalin);
    skaeth->addMeleeWeapon(&m_javalinMelee);
    skaeth->setName("Skaeth");
    addModel(skaeth);

    auto althaen = new Model(BASESIZE, wounds());
    althaen->addMissileWeapon(&m_seekerBow);
    althaen->addMeleeWeapon(&m_huntingWeapon);
    althaen->setName("Althaen");
    addModel(althaen);

    auto karthaen = new Model(BASESIZE, wounds());
    karthaen->addMeleeWeapon(&m_huntingWeapon);
    karthaen->setName("Karthaen");
    addModel(karthaen);

    auto sheoch = new Model(BASESIZE, wounds());
    sheoch->addMeleeWeapon(&m_huntingWeapon);
    sheoch->setName("Sheoch");
    addModel(sheoch);

    auto lighaen = new Model(BASESIZE, wounds());
    lighaen->addMeleeWeapon(&m_teethAndClaws);
    lighaen->setName("Lighaen");
    addModel(lighaen);

    m_points = POINTS_PER_UNIT;

    return true;
}

int SkaethsWildHunt::ComputePoints(int numModels)
{
    return POINTS_PER_UNIT;
}

} // namespace Sylvaneth