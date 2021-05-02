/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fyreslayers/AuricRunesmiter.h>
#include <Board.h>
#include <UnitFactory.h>
#include <fyreslayers/ZharrgrimBlessing.h>
#include "FyreslayerPrivate.h"

namespace Fyreslayers {

    class RunicEmpowerment : public Prayer {
    public:
        explicit RunicEmpowerment(Unit *priest, int range) :
                Prayer(priest, "Runic Empowerment", 3, range, 0) {
            m_allowedTargets = Abilities::Target::Friendly;
            m_effect = Abilities::EffectType::Buff;
        }

    protected:

        bool apply(int prayingRoll, Unit *target) override {
            if (target == nullptr) return false;
            target->buffReroll(Attribute::To_Wound_Melee, Rerolls::Failed,
                               {GamePhase::Hero, m_round + 1, m_priest->owningPlayer()});
            target->buffReroll(Attribute::To_Wound_Missile, Rerolls::Failed,
                               {GamePhase::Hero, m_round + 1, m_priest->owningPlayer()});
            return true;
        }

        bool apply(int prayingRoll, double x, double y) override { return false; }
    };

    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 120;

    bool AuricRunesmiter::s_registered = false;

    AuricRunesmiter::AuricRunesmiter() :
            Fyreslayer("Auric Runesmiter", 4, g_wounds, 7, 5, false, g_pointsPerUnit),
            m_throwingAxe(Weapon::Type::Missile, "Fyresteel Throwing Axe", 8, 1, 5, 5, 0, 1),
            m_runicIron(Weapon::Type::Melee, "Runic Iron", 1, 2, 3, 4, 0, 1),
            m_latchAxe(Weapon::Type::Melee, "Latch-axe", 1, 1, 4, 3, 0, 2) {
        m_keywords = {ORDER, DUARDIN, FYRESLAYERS, HERO, PRIEST, AURIC_RUNESMITER};
        m_weapons = {&m_throwingAxe, &m_runicIron, &m_latchAxe};
        m_battleFieldRole = Role::Leader;
    }

    void AuricRunesmiter::configure(Blessing blessing, WeaponOption weaponOption) {

        m_weaponOption = weaponOption;

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_throwingAxe);
        if (weaponOption == Runic_Iron) {
            model->addMeleeWeapon(&m_runicIron);
        }
        model->addMeleeWeapon(&m_latchAxe);
        addModel(model);

        m_knownPrayers.push_back(std::make_unique<RunicEmpowerment>(this, (m_weaponOption == Forge_Key) ? 18 : 12));
        m_knownPrayers.push_back(std::unique_ptr<Prayer>(CreateZharrgrimBlessing(blessing, this)));

        m_points = g_pointsPerUnit;
    }

    Unit *AuricRunesmiter::Create(const ParameterList &parameters) {
        auto unit = new AuricRunesmiter();

        auto lodge = (Lodge) GetEnumParam("Lodge", parameters, g_lodge[0]);
        unit->setLodge(lodge);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_masterSmiterTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_forgeTempleArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto prayer = (Blessing) GetEnumParam("Prayer", parameters, g_prayers[0]);
        auto weapons = (WeaponOption) GetEnumParam("Weapons", parameters, Runic_Iron);

        unit->configure(prayer, weapons);
        return unit;
    }

    std::string AuricRunesmiter::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == Runic_Iron) { return "Runic Iron"; }
            else if (parameter.intValue == Forge_Key) { return "Forge Key"; }
        }
        return Fyreslayer::ValueToString(parameter);
    }

    int AuricRunesmiter::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Runic Iron") { return Runic_Iron; }
        else if (enumString == "Forge Key") { return Forge_Key; }
        return Fyreslayer::EnumStringToInt(enumString);
    }

    void AuricRunesmiter::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Runic_Iron, Forge_Key};
            static FactoryMethod factoryMethod = {
                    AuricRunesmiter::Create,
                    AuricRunesmiter::ValueToString,
                    AuricRunesmiter::EnumStringToInt,
                    AuricRunesmiter::ComputePoints,
                    {
                            EnumParameter("Weapons", Runic_Iron, weapons),
                            EnumParameter("Lodge", g_lodge[0], g_lodge),
                            EnumParameter("Command Trait", g_masterSmiterTraits[0], g_masterSmiterTraits),
                            EnumParameter("Artefact", g_forgeTempleArtefacts[0], g_forgeTempleArtefacts),
                            EnumParameter("Prayer", g_prayers[0], g_prayers),
                            BoolParameter("General")
                    },
                    ORDER,
                    {FYRESLAYERS}
            };
            s_registered = UnitFactory::Register("Auric Runesmiter", factoryMethod);
        }
    }

    int AuricRunesmiter::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace Fyreslayers