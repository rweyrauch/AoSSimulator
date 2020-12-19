/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <khorne/Skullreapers.h>
#include <UnitFactory.h>
#include <Board.h>
#include "KhornePrivate.h"

namespace Khorne {
    static const int g_basesize = 40;
    static const int g_wounds = 3;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 20;
    static const int g_pointsPerBlock = 180;
    static const int g_pointsMaxUnitSize = 720;

    bool Skullreapers::s_registered = false;

    Skullreapers::Skullreapers() :
            KhorneBase("Skullreapers", 5, g_wounds, 7, 4, false),
            m_blades(Weapon::Type::Melee, "Gore-slick Blades, Daemonblade, Spinecleavers and Soultearers", 1, 4, 3, 3,
                     0, 1),
            m_viciousMutation(Weapon::Type::Melee, "Vicious Mutation", 1, 1, 3, 4, -1, RAND_D3) {
        m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, SKULLREAPERS};
        m_weapons = {&m_blades, &m_viciousMutation};
    }

    bool Skullreapers::configure(int numModels, bool iconBearer) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        // TODO: make this buffer dynamic
        if (iconBearer) {
            m_bravery += 1;
        }

        auto skullseeker = new Model(g_basesize, wounds());
        skullseeker->addMeleeWeapon(&m_viciousMutation);
        skullseeker->addMeleeWeapon(&m_blades);
        addModel(skullseeker);

        int currentModelCount = (int) m_models.size();
        for (auto i = currentModelCount; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_blades);
            if (iconBearer) {
                model->setName("Icon Bearer");
                iconBearer = false;
            }
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *Skullreapers::Create(const ParameterList &parameters) {
        auto unit = new Skullreapers();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool iconBearer = GetBoolParam("Icon Bearer", parameters, true);

        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, g_slaughterHost[0]);
        unit->setSlaughterHost(host);

        bool ok = unit->configure(numModels, iconBearer);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Skullreapers::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Skullreapers::Create,
                    KhorneBase::ValueToString,
                    KhorneBase::EnumStringToInt,
                    Skullreapers::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            BoolParameter("Icon Bearer"),
                            EnumParameter("Slaughter Host", g_slaughterHost[0], g_slaughterHost)
                    },
                    CHAOS,
                    {KHORNE}
            };
            s_registered = UnitFactory::Register("Skullreapers", factoryMethod);
        }
    }

    Rerolls Skullreapers::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Trial of skulls.
        if (target->remainingModels() >= 5) {
            return Reroll_Failed;
        }
        return KhorneBase::toHitRerolls(weapon, target);
    }

    Wounds Skullreapers::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Daemonforged Weapons
        if ((weapon->name() == m_blades.name()) && (hitRoll == 6)) {
            Wounds wounds = {weapon->damage(), 1};
            return wounds;
        }
        return KhorneBase::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int Skullreapers::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    void Skullreapers::onModelSlain(Wounds::Source source) {
        Unit::onModelSlain(source);

        // Murderous to the Last
        if (source == Wounds::Source::Weapon_Melee) {
            auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
            if (unit && distanceTo(unit) <= 1.0) {
                if (Dice::RollD6() >= 5) {
                    unit->applyDamage({0, Dice::RollD3()}, this);
                }
            }
        }
    }

} //namespace Khorne