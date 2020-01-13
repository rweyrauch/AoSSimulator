#include <UnitFactory.h>
#include "sylvaneth/Ylthari.h"

namespace Sylvaneth
{
static FactoryMethod factoryMethod = {
    Ylthari::Create,
    SylvanethBase::ValueToString,
    SylvanethBase::EnumStringToInt,
    {
    },
    ORDER,
    { SYLVANETH }
};

bool Ylthari::s_registered = false;


Unit *Ylthari::Create(const ParameterList &parameters)
{
    auto unit = new Ylthari();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Ylthari::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Ylthari", factoryMethod);
    }
}

Ylthari::Ylthari() :
    SylvanethBase("Ylthari", 5, WOUNDS, 7, 5, false),
    m_briarStaff(Weapon::Type::Melee, "Briar Staff", 1, 1, 3, 3, -1, RAND_D3),
    m_thorns(Weapon::Type::Melee, "Spiteful Thorns", 1, 3, 4, 4, 0, 1),
    m_snappingMandibles(Weapon::Type::Melee, "Snapping Mandibles", 1, 1, 4, 4, -1, 1)
{
    m_keywords = {ORDER, SYLVANETH, OAKENBROW, HERO, WIZARD, THORNWYCH, YLTHARI};
    m_weapons = {&m_briarStaff, &m_thorns, &m_snappingMandibles};
}

bool Ylthari::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_briarStaff);
    model->addMeleeWeapon(&m_thorns);
    model->addMeleeWeapon(&m_snappingMandibles);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

} // namespace Sylvaneth