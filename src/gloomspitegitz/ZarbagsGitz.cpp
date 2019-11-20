/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <gloomspitegitz/ZarbagsGitz.h>
#include <UnitFactory.h>
#include <Board.h>

namespace GloomspiteGitz
{

static FactoryMethod factoryMethod = {
    ZarbagsGitz::Create,
    nullptr,
    nullptr,
    {
    },
    DESTRUCTION,
    GLOOMSPITE_GITZ
};

bool ZarbagsGitz::s_registered = false;

ZarbagsGitz::ZarbagsGitz() :
    GloomspiteGitzBase("Zarbag's Gitz", 5, WOUNDS, 4, 6, false),
    m_bow(Weapon::Type::Missile, "Moonclan Bow", 16, 1, 5, 5, 0, 1),
    m_teeth(Weapon::Type::Melee, "Massive Gob Full of Teeth", 1, 2, 4, 3, -1, 1),
    m_prodder(Weapon::Type::Melee, "Squig Prodder", 2, 1, 5, 4, 0, 1),
    m_slitta(Weapon::Type::Melee, "Slitta", 1, 1, 5, 5, 0, 1)
{
    m_keywords = {DESTRUCTION, GROT, GLOOMSPITE_GITZ, MOONCLAN, ZARBAGS_GITZ};
}

bool ZarbagsGitz::configure()
{
    Model herder(BASESIZE, SQUIG_WOUNDS);
    herder.addMeleeWeapon(&m_prodder);
    herder.setName("Herder");
    addModel(herder);

    Model squig(BASESIZE, SQUIG_WOUNDS);
    squig.addMeleeWeapon(&m_teeth);
    squig.setName("Squig");
    // Two squigs
    addModel(squig);
    addModel(squig);

    Model netter(BASESIZE, WOUNDS);
    netter.addMeleeWeapon(&m_slitta);
    netter.setName("Netter");
    addModel(netter);

    Model shoota(BASESIZE, WOUNDS);
    shoota.addMissileWeapon(&m_bow);
    shoota.addMeleeWeapon(&m_slitta);
    shoota.setName("Shoota");
    // Three shootas
    addModel(shoota);
    addModel(shoota);
    addModel(shoota);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *ZarbagsGitz::Create(const ParameterList &parameters)
{
    auto unit = new ZarbagsGitz();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void ZarbagsGitz::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Zarbag's Gitz", factoryMethod);
    }
}

void ZarbagsGitz::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_bow);
    visitor(m_teeth);
    visitor(m_prodder);
    visitor(m_slitta);
}

int ZarbagsGitz::targetHitModifier(const Weapon *weapon, const Unit *attacker) const
{
    int modifier = GloomspiteGitzBase::targetHitModifier(weapon, attacker);
    // Netters
    if (distanceTo(attacker) <= 2.0f)
    {
        bool hasNets = false;
        for (auto ip : m_models)
        {
            if (ip.getName() == "Netter")
            {
                hasNets = true;
                break;
            }
        }
        if (hasNets)
        {
            modifier -= 1;
        }
    }
    return modifier;
}

} // namespace GloomspiteGitz