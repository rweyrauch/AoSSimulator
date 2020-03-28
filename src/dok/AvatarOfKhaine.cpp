/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <dok/AvatarOfKhaine.h>
#include <UnitFactory.h>

namespace DaughtersOfKhaine {
    static const int BASESIZE = 40;
    static const int WOUNDS = 9;
    static const int POINTS_PER_UNIT = 140;

    bool AvatarOfKhaine::s_registered = false;

    AvatarOfKhaine::AvatarOfKhaine() :
            DaughterOfKhaine("Avatar of Khaine", 9, WOUNDS, 10, 4, false),
            m_torrentOfBurningBlood(Weapon::Type::Missile, "Torrent of Burning Blood", 10, 6, 3, 3, -1, 1),
            m_sword(Weapon::Type::Melee, "Avatar of Khaine's Sword", 2, 4, 3, 3, -2, 3) {
        m_keywords = {ORDER, DAUGHTERS_OF_KHAINE, TOTEM, AVATAR_OF_KHAINE};
        m_weapons = {&m_torrentOfBurningBlood, &m_sword};

        s_globalBraveryMod.connect(this, &AvatarOfKhaine::idolOfWorship, &m_idolSlot);
    }

    AvatarOfKhaine::~AvatarOfKhaine() {
        m_idolSlot.disconnect();
    }

    bool AvatarOfKhaine::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_torrentOfBurningBlood);
        model->addMeleeWeapon(&m_sword);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *AvatarOfKhaine::Create(const ParameterList &parameters) {
        auto unit = new AvatarOfKhaine();

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void AvatarOfKhaine::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    nullptr,
                    nullptr,
                    ComputePoints,
                    {
                    },
                    ORDER,
                    {DAUGHTERS_OF_KHAINE}
            };
            s_registered = UnitFactory::Register("Avatar of Khaine", factoryMethod);
        }
    }

    int AvatarOfKhaine::idolOfWorship(const Unit *unit) {
        // Idol of Worship
        if (unit->hasKeyword(DAUGHTERS_OF_KHAINE) && (distanceTo(unit) <= 7.0f)) return 1;

        return 0;
    }

    int AvatarOfKhaine::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} //namespace DaughtersOfKhaine

