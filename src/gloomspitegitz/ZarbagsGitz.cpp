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

namespace GloomspiteGitz {
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_squigWounds = 2;
    static const int g_pointsPerUnit = 80;

    bool ZarbagsGitz::s_registered = false;

    ZarbagsGitz::ZarbagsGitz() :
            GloomspiteGitzBase("Zarbag's Gitz", 5, g_wounds, 4, 6, false),
            m_bow(Weapon::Type::Missile, "Moonclan Bow", 16, 1, 5, 5, 0, 1),
            m_teeth(Weapon::Type::Melee, "Massive Gob Full of Teeth", 1, 2, 4, 3, -1, 1),
            m_prodder(Weapon::Type::Melee, "Squig Prodder", 2, 1, 5, 4, 0, 1),
            m_slitta(Weapon::Type::Melee, "Slitta", 1, 1, 5, 5, 0, 1) {
        m_keywords = {DESTRUCTION, GROT, GLOOMSPITE_GITZ, MOONCLAN, ZARBAGS_GITZ};
        m_weapons = {&m_bow, &m_teeth, &m_prodder, &m_slitta};
    }

    bool ZarbagsGitz::configure() {
        auto herder = new Model(g_basesize, g_squigWounds);
        herder->addMeleeWeapon(&m_prodder);
        herder->setName("Herder");
        addModel(herder);

        // Two squigs
        auto squig1 = new Model(g_basesize, g_squigWounds);
        squig1->addMeleeWeapon(&m_teeth);
        squig1->setName("Squig");
        addModel(squig1);

        auto squig2 = new Model(g_basesize, g_squigWounds);
        squig2->addMeleeWeapon(&m_teeth);
        squig2->setName("Squig");
        addModel(squig2);

        auto netter = new Model(g_basesize, wounds());
        netter->addMeleeWeapon(&m_slitta);
        netter->setName("Netter");
        addModel(netter);

        // Three shootas
        for (auto i = 0; i < 3; i++) {
            auto shoota = new Model(g_basesize, wounds());
            shoota->addMissileWeapon(&m_bow);
            shoota->addMeleeWeapon(&m_slitta);
            shoota->setName("Shoota");
            addModel(shoota);
        }

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *ZarbagsGitz::Create(const ParameterList &parameters) {
        auto unit = new ZarbagsGitz();

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void ZarbagsGitz::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    ZarbagsGitz::Create,
                    GloomspiteGitzBase::ValueToString,
                    GloomspiteGitzBase::EnumStringToInt,
                    ZarbagsGitz::ComputePoints,
                    {
                    },
                    DESTRUCTION,
                    {GLOOMSPITE_GITZ}
            };
            s_registered = UnitFactory::Register("Zarbag's Gitz", factoryMethod);
        }
    }

    int ZarbagsGitz::targetHitModifier(const Weapon *weapon, const Unit *attacker) const {
        int modifier = GloomspiteGitzBase::targetHitModifier(weapon, attacker);
        // Netters
        if (distanceTo(attacker) <= 2.0) {
            if (isNamedModelAlive("Netter")) {
                modifier -= 1;
            }
        }
        return modifier;
    }

    int ZarbagsGitz::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace GloomspiteGitz