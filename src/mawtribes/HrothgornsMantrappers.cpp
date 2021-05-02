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
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_pointsPerUnit = 40;

    bool HrothgornsMantrappers::s_registered = false;

    bool HrothgornsMantrappers::AreValid(const ParameterList &parameters) {
        return true;
    }

    Unit *HrothgornsMantrappers::Create(const ParameterList &parameters) {
        if (AreValid(parameters)) {
            return new HrothgornsMantrappers();
        }
        return nullptr;
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
            MawtribesBase(Mawtribe::Winterbite, "Hrothgorn's Mantrappers", 5, g_wounds, 4, 6, false, g_pointsPerUnit) {
        m_keywords = {DESTRUCTION, GROT, OGOR_MAWTRIBES, BEASTCLAW_RAIDERS, WINTERBITE, GNOBLARS,
                      HROTHGORNS_MANTRAPPERS};
        m_weapons = {&m_sharpStuff, &m_motleyWeapons};

        auto bushwakka = new Model(g_basesize, wounds());
        bushwakka->setName("Bushwakka");
        bushwakka->addMissileWeapon(&m_sharpStuff);
        bushwakka->addMeleeWeapon(&m_motleyWeapons);
        addModel(bushwakka);

        auto quiv = new Model(g_basesize, wounds());
        quiv->setName("Quiv");
        quiv->addMissileWeapon(&m_sharpStuff);
        quiv->addMeleeWeapon(&m_motleyWeapons);
        addModel(quiv);

        auto luggitAndThwak = new Model(g_basesize, wounds());
        luggitAndThwak->setName("Luggit and Thwak");
        luggitAndThwak->addMissileWeapon(&m_sharpStuff);
        luggitAndThwak->addMeleeWeapon(&m_motleyWeapons);
        addModel(luggitAndThwak);
    }

    int HrothgornsMantrappers::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace OgorMawtribes
