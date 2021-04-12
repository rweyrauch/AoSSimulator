/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "ossiarch/MortekCrawler.h"
#include "OssiarchBonereaperPrivate.h"

namespace OssiarchBonereapers {
    static const int g_basesize = 170; // x105 oval
    static const int g_wounds = 12;
    static const int g_pointsPerUnit = 200;

    struct TableEntry {
        int m_skullDamage;
        int m_cauldron;
        int m_stele;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {2, 4, 7, 9, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {5, 0,  -3},
                    {4, 0,  -1},
                    {3, -1, 0},
                    {2, -1, 1},
                    {1, -2, 3}
            };

    bool MortekCrawler::s_registered = false;

    Unit *MortekCrawler::Create(const ParameterList &parameters) {
        auto legion = (Legion) GetEnumParam("Legion", parameters, g_legion[0]);
        return new MortekCrawler(legion);
    }

    void MortekCrawler::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    MortekCrawler::Create,
                    OssiarchBonereaperBase::ValueToString,
                    OssiarchBonereaperBase::EnumStringToInt,
                    MortekCrawler::ComputePoints,
                    {
                            EnumParameter("Legion", g_legion[0], g_legion),
                    },
                    DEATH,
                    {OSSIARCH_BONEREAPERS}
            };
            s_registered = UnitFactory::Register("Mortek Crawler", factoryMethod);
        }
    }

    MortekCrawler::MortekCrawler(Legion legion) :
            OssiarchBonereaperBase("Mortek Crawler", 4, g_wounds, 10, 4, false) {
        m_keywords = {DEATH, OSSIARCH_BONEREAPERS, WAR_MACHINE, MORTEK_CRAWLER};
        m_weapons = {&m_catapultSkulls, &m_catapultCauldron, &m_catapultSkulls, &m_tools};
        m_battleFieldRole = Role::Artillery_Behemoth;

        setLegion(legion);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_catapultSkulls);
        model->addMissileWeapon(&m_catapultCauldron);
        model->addMissileWeapon(&m_catapultStele);
        model->addMeleeWeapon(&m_tools);

        m_catapultCauldron.activate(false);
        m_catapultStele.activate(false);

        addModel(model);

        m_points = g_pointsPerUnit;
    }

    void MortekCrawler::onWounded() {
        const auto damageIndex = getDamageTableIndex();
        m_catapultSkulls.setDamage(g_damageTable[damageIndex].m_skullDamage);

        OssiarchBonereaperBase::onWounded();
    }

    size_t MortekCrawler::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    int MortekCrawler::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    void MortekCrawler::onRestore() {
        OssiarchBonereaperBase::onRestore();

        // Restore table-driven attributes
        onWounded();

        m_usedCauldronOfTorment = false;
        m_usedCursedSteel = false;
    }

} // namespace OssiarchBonereapers
