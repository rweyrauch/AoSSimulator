/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "mawtribes/HrothgornsMantrappers.h"

namespace OgorMawtribes {
    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int POINTS_PER_UNIT = 40;

    bool HrothgornsMantrappers::s_registered = false;

    Unit *HrothgornsMantrappers::Create(const ParameterList &parameters) {
        auto unit = new HrothgornsMantrappers();

        unit->setMawtribe(Winterbite);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void HrothgornsMantrappers::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    HrothgornsMantrappers::Create,
                    MawtribesBase::ValueToString,
                    MawtribesBase::EnumStringToInt,
                    HrothgornsMantrappers::ComputePoints,
                    {
                    },
                    DESTRUCTION,
                    {OGOR_MAWTRIBES}
            };
            s_registered = UnitFactory::Register("Hrothgorn's Mantrappers", factoryMethod);
        }
    }

    HrothgornsMantrappers::HrothgornsMantrappers() :
            MawtribesBase("Hrothgorn's Mantrappers", 5, WOUNDS, 4, 6, false),
            m_sharpStuff(Weapon::Type::Missile, "Sharp Stuff", 8, 1, 4, 5, 0, 1),
            m_motleyWeapons(Weapon::Type::Melee, "Motley Assortment of Weapons", 1, 1, 5, 5, 0, 1) {
        m_keywords = {DESTRUCTION, GROT, OGOR_MAWTRIBES, BEASTCLAW_RAIDERS, WINTERBITE, GNOBLARS,
                      HROTHGORNS_MANTRAPPERS};
        m_weapons = {&m_sharpStuff, &m_motleyWeapons};
    }

    bool HrothgornsMantrappers::configure() {
        auto bushwakka = new Model(BASESIZE, wounds());
        bushwakka->setName("Bushwakka");
        bushwakka->addMissileWeapon(&m_sharpStuff);
        bushwakka->addMeleeWeapon(&m_motleyWeapons);
        addModel(bushwakka);

        auto quiv = new Model(BASESIZE, wounds());
        quiv->setName("Quiv");
        quiv->addMissileWeapon(&m_sharpStuff);
        quiv->addMeleeWeapon(&m_motleyWeapons);
        addModel(quiv);

        auto luggitAndThwak = new Model(BASESIZE, wounds());
        luggitAndThwak->setName("Luggit and Thwak");
        luggitAndThwak->addMissileWeapon(&m_sharpStuff);
        luggitAndThwak->addMeleeWeapon(&m_motleyWeapons);
        addModel(luggitAndThwak);

        m_points = ComputePoints(1);

        return true;
    }

    int HrothgornsMantrappers::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace OgorMawtribes
