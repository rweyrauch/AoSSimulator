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
    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int SQUIG_WOUNDS = 2;
    static const int POINTS_PER_UNIT = 80;

    bool ZarbagsGitz::s_registered = false;

    ZarbagsGitz::ZarbagsGitz() :
            GloomspiteGitzBase("Zarbag's Gitz", 5, WOUNDS, 4, 6, false),
            m_bow(Weapon::Type::Missile, "Moonclan Bow", 16, 1, 5, 5, 0, 1),
            m_teeth(Weapon::Type::Melee, "Massive Gob Full of Teeth", 1, 2, 4, 3, -1, 1),
            m_prodder(Weapon::Type::Melee, "Squig Prodder", 2, 1, 5, 4, 0, 1),
            m_slitta(Weapon::Type::Melee, "Slitta", 1, 1, 5, 5, 0, 1) {
        m_keywords = {DESTRUCTION, GROT, GLOOMSPITE_GITZ, MOONCLAN, ZARBAGS_GITZ};
        m_weapons = {&m_bow, &m_teeth, &m_prodder, &m_slitta};
    }

    bool ZarbagsGitz::configure() {
        auto herder = new Model(BASESIZE, SQUIG_WOUNDS);
        herder->addMeleeWeapon(&m_prodder);
        herder->setName("Herder");
        addModel(herder);

        // Two squigs
        auto squig1 = new Model(BASESIZE, SQUIG_WOUNDS);
        squig1->addMeleeWeapon(&m_teeth);
        squig1->setName("Squig");
        addModel(squig1);

        auto squig2 = new Model(BASESIZE, SQUIG_WOUNDS);
        squig2->addMeleeWeapon(&m_teeth);
        squig2->setName("Squig");
        addModel(squig2);

        auto netter = new Model(BASESIZE, wounds());
        netter->addMeleeWeapon(&m_slitta);
        netter->setName("Netter");
        addModel(netter);

        // Three shootas
        for (auto i = 0; i < 3; i++) {
            auto shoota = new Model(BASESIZE, wounds());
            shoota->addMissileWeapon(&m_bow);
            shoota->addMeleeWeapon(&m_slitta);
            shoota->setName("Shoota");
            addModel(shoota);
        }

        m_points = POINTS_PER_UNIT;

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
            bool hasNets = false;
            for (auto &ip : m_models) {
                if (ip->getName() == "Netter") {
                    hasNets = true;
                    break;
                }
            }
            if (hasNets) {
                modifier -= 1;
            }
        }
        return modifier;
    }

    int ZarbagsGitz::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace GloomspiteGitz