/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "ossiarch/Boneshaper.h"
#include "OssiarchBonereaperPrivate.h"

namespace OssiarchBonereapers {
    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 130;

    bool MortisanBoneshaper::s_registered = false;

    Unit *MortisanBoneshaper::Create(const ParameterList &parameters) {
        auto unit = new MortisanBoneshaper();

        auto legion = (Legion) GetEnumParam("Legion", parameters, g_legion[0]);
        unit->setLegion(legion);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_mortisanCommandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_boneshaperArtefacts[0]);
        unit->setArtefact(artefact);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string MortisanBoneshaper::ValueToString(const Parameter &parameter) {
        return OssiarchBonereaperBase::ValueToString(parameter);
    }

    int MortisanBoneshaper::EnumStringToInt(const std::string &enumString) {
        return OssiarchBonereaperBase::EnumStringToInt(enumString);
    }

    void MortisanBoneshaper::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    MortisanBoneshaper::Create,
                    MortisanBoneshaper::ValueToString,
                    MortisanBoneshaper::EnumStringToInt,
                    MortisanBoneshaper::ComputePoints,
                    {
                            EnumParameter("Legion", g_legion[0], g_legion),
                            EnumParameter("Lore", g_lore[0], g_lore),
                            EnumParameter("Command Trait", g_mortisanCommandTraits[0], g_mortisanCommandTraits),
                            EnumParameter("Artefact", g_boneshaperArtefacts[0], g_boneshaperArtefacts),
                    },
                    DEATH,
                    {OSSIARCH_BONEREAPERS}
            };
            s_registered = UnitFactory::Register("Mortisan Boneshaper", factoryMethod);
        }
    }

    MortisanBoneshaper::MortisanBoneshaper() :
            OssiarchBonereaperBase("Mortisan Boneshaper", 5, WOUNDS, 10, 4, false),
            m_talons(Weapon::Type::Melee, "Ossified Talons", 1, 2, 3, 4, 0, 1) {
        m_keywords = {DEATH, OSSIARCH_BONEREAPERS, MORTISAN, HERO, WIZARD, MORTISAN_BONESHAPER};
        m_weapons = {&m_talons};

        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    bool MortisanBoneshaper::configure(Lore lore) {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_talons);
        addModel(model);

        m_lore = lore;

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = POINTS_PER_UNIT;

        return true;
    }

    int MortisanBoneshaper::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace OssiarchBonereapers
