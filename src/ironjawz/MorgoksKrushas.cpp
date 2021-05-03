/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <ironjawz/MorgoksKrushas.h>
#include <UnitFactory.h>

namespace Ironjawz {
    static const int g_basesize = 40;
    static const int g_wounds = 3;
    static const int g_pointsPerUnit = 90;

    bool MorgoksKrushas::s_registered = false;

    MorgoksKrushas::MorgoksKrushas() :
            Ironjawz(Warclan::Ironsunz, "Morgok's Krushas", 4, g_wounds, 6, 4, false, g_pointsPerUnit),
            m_krushaWeaponsMorgok(Weapon::Type::Melee, "Krusha Weapons", 1, 5, 3, 3, -1, 1),
            m_krushaWeapons(Weapon::Type::Melee, "Krusha Weapons", 1, 4, 3, 3, -1, 1),
            m_goreBasha(Weapon::Type::Melee, "Gore-basha", 2, 3, 4, 3, -1, 2) {
        m_keywords = {DESTRUCTION, ORRUK, IRONJAWZ, BRUTES, MORGOKS_KRUSHAS};
        m_weapons = {&m_krushaWeaponsMorgok, &m_krushaWeapons, &m_goreBasha};

        auto bossModel = new Model(g_basesize, wounds());
        bossModel->addMeleeWeapon(&m_krushaWeaponsMorgok);
        bossModel->setName("Morgok");
        addModel(bossModel);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_krushaWeapons);
        model->setName("Thugg");
        addModel(model);

        auto ardskull = new Model(g_basesize, wounds());
        ardskull->addMeleeWeapon(&m_goreBasha);
        ardskull->setName("Ardskull");
        addModel(ardskull);
    }

    Unit *MorgoksKrushas::Create(const ParameterList &parameters) {
        return new MorgoksKrushas();
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
            return Rerolls::Failed;
        }
        return Ironjawz::toHitRerolls(weapon, target);
    }

    int MorgoksKrushas::ComputePoints(const ParameterList& parameters) {
        return g_pointsPerUnit;
    }

} // namespace Ironjawz
