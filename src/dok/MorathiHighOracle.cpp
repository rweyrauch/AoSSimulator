/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <dok/MorathiHighOracle.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>

namespace DaughtersOfKhaine
{
bool MorathiHighOracleOfKhaine::s_registered = false;

MorathiHighOracleOfKhaine::MorathiHighOracleOfKhaine() :
    DaughterOfKhaine("Morathi, High Oracle of Khaine", 6, WOUNDS, 9, 4, false),
    m_heartrender(Weapon::Type::Melee, "Heartrender", 2, 3, 3, 3, -1, RAND_D3),
    m_wings(Weapon::Type::Melee, "Bladed Wings", 2, 6, 3, 3, -1, 1)
{
    m_keywords = {ORDER, AELF, DAUGHTERS_OF_KHAINE, HERO, WIZARD, MORATHI, HIGH_ORACLE_OF_KHAINE};
    m_weapons = {&m_heartrender, &m_wings};

    m_totalSpells = 3;
    m_totalUnbinds = 2;
}

bool MorathiHighOracleOfKhaine::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_heartrender);
    model->addMeleeWeapon(&m_wings);
    addModel(model);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *MorathiHighOracleOfKhaine::Create(const ParameterList &parameters)
{
    auto unit = new MorathiHighOracleOfKhaine();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void MorathiHighOracleOfKhaine::Init()
{
    if (!s_registered)
    {
        static auto factoryMethod = new FactoryMethod{
            MorathiHighOracleOfKhaine::Create,
            nullptr,
            nullptr,
            MorathiHighOracleOfKhaine::ComputePoints,
            {
            },
            ORDER,
            { DAUGHTERS_OF_KHAINE }
        };
        s_registered = UnitFactory::Register("Morathi, High Oracle of Khaine", *factoryMethod);
    }
}

} //namespace DaughtersOfKhaine

