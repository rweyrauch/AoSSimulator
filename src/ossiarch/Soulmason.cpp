/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "ossiarch/Soulmason.h"
#include "OssiarchBonereaperPrivate.h"

namespace OssiarchBonereapers {

    class SoulGuide : public Spell {
    public:
        explicit SoulGuide(Unit* caster);

    protected:
        Result apply(int castingValue, int unmodifiedCastingValue, Unit* target) override;
        Result apply(int castingValue, int unmodifiedCastingValue, double x, double y) override  { return Result::Failed; }
    };

    SoulGuide::SoulGuide(Unit *caster) :
            Spell(caster, "Soul-guide", 6, 24) {
        m_allowedTargets = Abilities::Target::SelfAndFriendly;
        m_effect = Abilities::EffectType::Buff;
        m_targetKeywords.push_back(OSSIARCH_BONEREAPERS);
    }

    Spell::Result SoulGuide::apply(int castingValue, int unmodifiedCastingValue, Unit* target) {
        if (target == nullptr) {
            return Spell::Result::Failed;
        }

        target->buffReroll(To_Hit_Melee, Reroll_Ones, defaultDuration());
        target->buffReroll(To_Hit_Missile, Reroll_Ones, defaultDuration());

        return Spell::Result::Success;
    }

    static const int g_basesize = 40;
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 140;

    bool MortisanSoulmason::s_registered = false;

    Unit *MortisanSoulmason::Create(const ParameterList &parameters) {
        auto unit = new MortisanSoulmason();

        auto legion = (Legion) GetEnumParam("Legion", parameters, g_legion[0]);
        unit->setLegion(legion);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_mortisanCommandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_soulmasonArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string MortisanSoulmason::ValueToString(const Parameter &parameter) {
        return OssiarchBonereaperBase::ValueToString(parameter);
    }

    int MortisanSoulmason::EnumStringToInt(const std::string &enumString) {
        return OssiarchBonereaperBase::EnumStringToInt(enumString);
    }

    void MortisanSoulmason::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    MortisanSoulmason::Create,
                    MortisanSoulmason::ValueToString,
                    MortisanSoulmason::EnumStringToInt,
                    MortisanSoulmason::ComputePoints,
                    {
                            EnumParameter("Legion", g_legion[0], g_legion),
                            EnumParameter("Lore", g_lore[0], g_lore),
                            EnumParameter("Command Trait", g_mortisanCommandTraits[0], g_mortisanCommandTraits),
                            EnumParameter("Artefact", g_soulmasonArtefacts[0], g_soulmasonArtefacts),
                            BoolParameter("General")
                    },
                    DEATH,
                    {OSSIARCH_BONEREAPERS}
            };
            s_registered = UnitFactory::Register("Mortisan Soulmason", factoryMethod);
        }
    }

    MortisanSoulmason::MortisanSoulmason() :
            OssiarchBonereaperBase("Mortisan Soulmason", 5, g_wounds, 10, 5, false),
            m_staff(Weapon::Type::Melee, "Soulmason's Staff", 2, 2, 4, 3, -1, RAND_D3),
            m_claws(Weapon::Type::Melee, "Ossified Claws", 1, 2, 4, 3, -1, 1) {
        m_keywords = {DEATH, OSSIARCH_BONEREAPERS, MORTISAN, HERO, WIZARD, MORTISAN_SOULMASON};
        m_weapons = {&m_staff, &m_claws};
        m_battleFieldRole = Leader;

        m_totalSpells = 2;
        m_totalUnbinds = 2;
    }

    bool MortisanSoulmason::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_staff);
        model->addMeleeWeapon(&m_claws);
        addModel(model);

        m_lore = lore;

        m_knownSpells.push_back(std::make_unique<SoulGuide>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;

        return true;
    }

    int MortisanSoulmason::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace OssiarchBonereapers
