/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <gloomspitegitz/Skragrott.h>
#include <UnitFactory.h>
#include <iostream>
#include <Board.h>
#include <spells/MysticShield.h>
#include <spells/NikkitNikkit.h>

namespace GloomspiteGitz
{
static FactoryMethod factoryMethod = {
    Skragrott::Create,
    nullptr,
    nullptr,
    {
    },
    DESTRUCTION,
    GLOOMSPITE_GITZ
};

bool Skragrott::s_registered = false;

Skragrott::Skragrott() :
    GloomspiteGitzBase("Skragrott", 4, WOUNDS, 6, 5, false),
    m_daMoonOnnaStikkMissile(Weapon::Type::Missile, "Puff Spores", 8, 1, 5, 5, 0, RAND_D3),
    m_daMoonOnnaStikk(Weapon::Type::Melee, "Enormous Jaws", 3, 8, 2, 3, -2, RAND_D3)
{
    m_keywords = {DESTRUCTION, GROT, GLOOMSPITE_GITZ, MOONCLAN, HERO, WIZARD, LOONBOSS, SKRAGROTT};

    m_totalUnbinds = 2;
    m_totalSpells = 2;
}

bool Skragrott::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMissileWeapon(&m_daMoonOnnaStikkMissile);
    model.addMeleeWeapon(&m_daMoonOnnaStikk);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    m_knownSpells.push_back(std::make_unique<NikkitNikkit>(this));

    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *Skragrott::Create(const ParameterList &parameters)
{
    auto unit = new Skragrott();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Skragrott::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Skragrott", factoryMethod);
    }
}

void Skragrott::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_daMoonOnnaStikkMissile);
    visitor(&m_daMoonOnnaStikk);
}

} // namespace GloomspiteGitz
