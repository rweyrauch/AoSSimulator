/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <gloomspitegitz/SpiderRiders.h>
#include <UnitFactory.h>

namespace GloomspiteGitz
{
static FactoryMethod factoryMethod = {
    SpiderRiders::Create,
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "Models", {.m_intValue = SpiderRiders::MIN_UNIT_SIZE},
            SpiderRiders::MIN_UNIT_SIZE, SpiderRiders::MAX_UNIT_SIZE, SpiderRiders::MIN_UNIT_SIZE
        },
        { ParamType::Boolean, "Drummers", {.m_boolValue = false}, false, false, false},
        { ParamType::Boolean, "Totem Bearers", {.m_boolValue = false}, false, false, false}
    },
    DESTRUCTION,
    GLOOMSPITE_GITZ
};

bool SpiderRiders::s_registered = false;


SpiderRiders::SpiderRiders() :
    GloomspiteGitzBase("Spider Riders", 10, WOUNDS, 4, 5, true), // Wall Crawler treated as fly
    m_spiderBow(Weapon::Type::Missile, "Spider-bow", 16, 2, 5, 5, 0, 1),
    m_crookedSpear(Weapon::Type::Melee, "Crooked Spear", 2, 1, 5, 4, 0, 1),
    m_crookedSpearBoss(Weapon::Type::Melee, "Crooked Spear (Boss)", 2, 1, 4, 4, 0, 1),
    m_fangs(Weapon::Type::Melee, "Fangs", 1, 2, 4, 4, 0, 1)
{
    m_keywords = {DESTRUCTION, GLOOMSPITE_GITZ, SPIDERFANG, GROT, SPIDER_RIDERS};
}

bool SpiderRiders::configure(int numModels, bool drummers, bool totemBearers)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    m_drummers = drummers;
    m_totemBearers = totemBearers;

    // Add the boss
    Model boss(BASESIZE, WOUNDS);
    boss.addMissileWeapon(&m_spiderBow);
    boss.addMeleeWeapon(&m_crookedSpearBoss);
    boss.addMeleeWeapon(&m_fangs);
    addModel(boss);

    // and the rest
    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMissileWeapon(&m_spiderBow);
        model.addMeleeWeapon(&m_crookedSpear);
        model.addMeleeWeapon(&m_fangs);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void SpiderRiders::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_spiderBow);
    visitor(&m_crookedSpear);
    visitor(&m_crookedSpearBoss);
    visitor(&m_fangs);
}

Unit *SpiderRiders::Create(const ParameterList &parameters)
{
    auto unit = new SpiderRiders();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    bool drummers = GetBoolParam("Drummers", parameters, false);
    bool totemBearers = GetBoolParam("Totem Bearers", parameters, false);

    bool ok = unit->configure(numModels, drummers, totemBearers);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void SpiderRiders::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Spider Riders", factoryMethod);
    }
}

int SpiderRiders::battlshockModifier() const
{
    int modifier = GloomspiteGitzBase::battlshockModifier();
    if (m_totemBearers)
        modifier += 2;
    return modifier;
}

int SpiderRiders::runModifier() const
{
    int modifier = GloomspiteGitzBase::runModifier();
    if (m_drummers)
        modifier += 2;
    return modifier;
}

Wounds SpiderRiders::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Spider Venom
    if (weapon->name() == m_fangs.name() && (hitRoll == 6))
    {
        return {weapon->damage(), 1};
    }
    return GloomspiteGitzBase::weaponDamage(weapon, target, hitRoll, woundRoll);
}

} //namespace GloomspiteGitz