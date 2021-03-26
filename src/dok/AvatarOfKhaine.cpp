/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <dok/AvatarOfKhaine.h>
#include <UnitFactory.h>
#include "DaughterOfKhainePrivate.h"

namespace DaughtersOfKhaine {
    static const int g_basesize = 40;
    static const int g_wounds = 9;
    static const int g_pointsPerUnit = 130;

    bool AvatarOfKhaine::s_registered = false;

    AvatarOfKhaine::AvatarOfKhaine() :
            DaughterOfKhaine("Avatar of Khaine", 9, g_wounds, 10, 4, false),
            m_torrentOfBurningBlood(Weapon::Type::Missile, "Torrent of Burning Blood", 10, 6, 3, 3, -1, 1),
            m_sword(Weapon::Type::Melee, "Avatar of Khaine's Sword", 2, 4, 3, 3, -2, 3) {
        m_keywords = {ORDER, DAUGHTERS_OF_KHAINE, TOTEM, AVATAR_OF_KHAINE};
        m_weapons = {&m_torrentOfBurningBlood, &m_sword};
        m_battleFieldRole = Role::Behemoth;

        s_globalBraveryMod.connect(this, &AvatarOfKhaine::idolOfWorship, &m_idolSlot);
    }

    AvatarOfKhaine::~AvatarOfKhaine() {
        m_idolSlot.disconnect();
    }

    bool AvatarOfKhaine::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_torrentOfBurningBlood);
        model->addMeleeWeapon(&m_sword);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *AvatarOfKhaine::Create(const ParameterList &parameters) {
        auto unit = new AvatarOfKhaine();

        auto temple = (Temple) GetEnumParam("Temple", parameters, g_temple[0]);
        unit->setTemple(temple);

        unit->configure();
        return unit;
    }

    void AvatarOfKhaine::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    DaughterOfKhaine::ValueToString,
                    DaughterOfKhaine::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Temple", g_temple[0], g_temple)
                    },
                    ORDER,
                    {DAUGHTERS_OF_KHAINE}
            };
            s_registered = UnitFactory::Register("Avatar of Khaine", factoryMethod);
        }
    }

    int AvatarOfKhaine::idolOfWorship(const Unit *unit) {
        // Idol of Worship
        if (unit->hasKeyword(DAUGHTERS_OF_KHAINE) && (distanceTo(unit) <= 7.0)) return 1;

        return 0;
    }

    int AvatarOfKhaine::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    void AvatarOfKhaine::animate(bool animated) {
        m_sword.activate(animated);
        m_torrentOfBurningBlood.activate(animated);
        m_canMove = animated;
    }

    bool AvatarOfKhaine::isAnimated() const {
        return m_sword.isActive();
    }

    void AvatarOfKhaine::onRestore() {
        m_sword.activate(false);
        m_torrentOfBurningBlood.activate(false);
        m_canMove = false;
    }

} //namespace DaughtersOfKhaine

