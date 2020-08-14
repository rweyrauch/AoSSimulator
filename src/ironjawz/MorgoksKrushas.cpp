/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <ironjawz/MorgoksKrushas.h>
#include <UnitFactory.h>
#include "IronjawzPrivate.h"

namespace Ironjawz {
    static const int BASESIZE = 40;
    static const int WOUNDS = 3;
    static const int POINTS_PER_UNIT = 90;

    bool MorgoksKrushas::s_registered = false;

    MorgoksKrushas::MorgoksKrushas() :
            Ironjawz("Morgok's Krushas", 4, WOUNDS, 6, 4, false),
            m_krushaWeaponsMorgok(Weapon::Type::Melee, "Krusha Weapons", 1, 5, 3, 3, -1, 1),
            m_krushaWeapons(Weapon::Type::Melee, "Krusha Weapons", 1, 4, 3, 3, -1, 1),
            m_goreBasha(Weapon::Type::Melee, "Gore-basha", 2, 3, 4, 3, -1, 2) {
        m_keywords = {DESTRUCTION, ORRUK, IRONJAWZ, BRUTES, MORGOKS_KRUSHAS};
        m_weapons = {&m_krushaWeaponsMorgok, &m_krushaWeapons, &m_goreBasha};
    }

    bool MorgoksKrushas::configure() {

        auto bossModel = new Model(BASESIZE, wounds());
        bossModel->addMeleeWeapon(&m_krushaWeaponsMorgok);
        bossModel->setName("Morgok");
        addModel(bossModel);

        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_krushaWeapons);
        model->setName("Thugg");
        addModel(model);

        auto ardskull = new Model(BASESIZE, wounds());
        ardskull->addMeleeWeapon(&m_goreBasha);
        ardskull->setName("Ardskull");
        addModel(ardskull);

        m_points = ComputePoints(1);

        return true;
    }

    Unit *MorgoksKrushas::Create(const ParameterList &parameters) {
        auto unit = new MorgoksKrushas();

        unit->setWarclan(Warclan::Ironsunz);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void MorgoksKrushas::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    MorgoksKrushas::Create,
                    Ironjawz::ValueToString,
                    Ironjawz::EnumStringToInt,
                    MorgoksKrushas::ComputePoints,
                    {
                    },
                    DESTRUCTION,
                    {IRONJAWZ}
            };
            s_registered = UnitFactory::Register("Morgok's Krushas", factoryMethod);
        }
    }

    Rerolls MorgoksKrushas::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Duff Up da Big Thing
        if (target->wounds() >= 4) {
            return RerollFailed;
        }
        return Ironjawz::toHitRerolls(weapon, target);
    }

    int MorgoksKrushas::ComputePoints(int numModels) {
        return POINTS_PER_UNIT;
    }

} // namespace Ironjawz
