/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <Board.h>
#include "citiesofsigmar/FreeguildGeneralOnGriffon.h"
#include "CitiesOfSigmarPrivate.h"

namespace CitiesOfSigmar {

    class RousingBattleCry : public CommandAbility {
    public:
        explicit RousingBattleCry(Unit *source) :
                CommandAbility(source, "Rousing Battle Cry", 12, 12, GamePhase::Charge) {
            m_allowedTargets = Abilities::Target::SelfAndFriendly;
            m_targetKeywords = {FREEGUILD};
            m_effect = Abilities::EffectType::Buff;
        }


    protected:

        bool apply(Unit *target) override {
            if (target == nullptr)
                return false;

            m_source->buffModifier(Attribute::Charge_Distance, 1, defaultDuration());
            m_source->buffModifier(Attribute::To_Hit_Melee, 1, defaultDuration());

            auto units = Board::Instance()->getUnitsWithin(m_source, m_source->owningPlayer(), m_rangeGeneral);
            for (auto unit : units) {
                if (unit == m_source) continue;

                if (unit->hasKeyword(FREEGUILD) && (unit->remainingModels() > 0)) {
                    unit->buffModifier(Attribute::Charge_Distance, 1, defaultDuration());
                    unit->buffModifier(Attribute::To_Hit_Melee, 1, defaultDuration());
                }
            }
            return true;
        }

        bool apply(double x, double y) override { return false; }
    };

    static const int g_basesize = 105;
    static const int g_wounds = 13;
    static const int g_pointsPerUnit = 320;

    struct TableEntry {
        int m_move;
        int m_clawAttacks;
        int m_beakDamage;
    };

    const size_t g_numTableEntries = 5;
    const int g_woundThresholds[g_numTableEntries] = {2, 4, 7, 9, g_wounds};
    const TableEntry g_damageTable[g_numTableEntries] =
            {
                    {15, 6, 4},
                    {13, 5, 3},
                    {11, 4, 2},
                    {9,  3, 1},
                    {7,  2, 1}
            };

    bool FreeguildGeneralOnGriffon::s_registered = false;

    Unit *FreeguildGeneralOnGriffon::Create(const ParameterList &parameters) {
        auto unit = new FreeguildGeneralOnGriffon();

        bool shield = GetBoolParam("Freeguild Shield", parameters, true);
        auto weapon = (WeaponOption) GetEnumParam("Weapon", parameters, Lance);

        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        unit->setCity(city);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto drug = (Narcotic) GetEnumParam("Narcotic", parameters, g_narcotic[0]);
        unit->setNarcotic(drug);

        unit->configure(weapon, shield);
        return unit;
    }

    std::string FreeguildGeneralOnGriffon::ValueToString(const Parameter &parameter) {
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int FreeguildGeneralOnGriffon::EnumStringToInt(const std::string &enumString) {
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void FreeguildGeneralOnGriffon::Init() {
        if (!s_registered) {
            static const std::array<int, 3> weapons = {Rune_Sword, Greathammer, Lance};
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Weapon", Lance, weapons),
                            BoolParameter("Freeguild Shield"),
                            EnumParameter("City", g_city[0], g_city),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            EnumParameter("Narcotic", g_narcotic[0], g_narcotic),
                            BoolParameter("General")
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Freeguild General on Griffon", factoryMethod);
        }
    }

    FreeguildGeneralOnGriffon::FreeguildGeneralOnGriffon() :
            CitizenOfSigmar("Freeguild General on Griffon", 15, g_wounds, 7, 4, true, g_pointsPerUnit),
            m_runesword(Weapon::Type::Melee, "Sigmarite Runesword", 1, 5, 3, 4, -1, 2),
            m_greathammer(Weapon::Type::Melee, "Sigmarite Greathammer", 1, 3, 3, 3, -2, RAND_D3),
            m_lance(Weapon::Type::Melee, "Freeguild Lance", 2, 4, 3, 4, -1, 2),
            m_claws(Weapon::Type::Melee, "Razor Claws", 2, 6, 4, 3, -1, 2),
            m_beak(Weapon::Type::Melee, "Deadly Beak", 2, 2, 3, 3, -2, 4) {
        m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, FREEGUILD, MONSTER, HERO, FREEGUILD_GENERAL};
        m_weapons = {&m_runesword, &m_greathammer, &m_lance, &m_claws, &m_beak};
        m_battleFieldRole = Role::Leader_Behemoth;
        m_hasMount = true;
        m_claws.setMount(true);
        m_beak.setMount(true);
        s_globalBraveryMod.connect(this, &FreeguildGeneralOnGriffon::piercingBloodroar, &m_connection);
    }

    FreeguildGeneralOnGriffon::~FreeguildGeneralOnGriffon() {
        m_connection.disconnect();
    }

    bool FreeguildGeneralOnGriffon::configure(WeaponOption weapon, bool hasShield) {
        auto model = new Model(g_basesize, wounds());
        if (weapon == Rune_Sword)
            model->addMeleeWeapon(&m_runesword);
        else if (weapon == Greathammer)
            model->addMeleeWeapon(&m_greathammer);
        else if (weapon == Lance)
            model->addMeleeWeapon(&m_lance);
        addModel(model);

        m_shield = hasShield;

        m_commandAbilities.push_back(std::make_unique<RousingBattleCry>(this));

        m_points = g_pointsPerUnit;

        return true;
    }

    int FreeguildGeneralOnGriffon::toSaveModifier(const Weapon *weapon, const Unit *attacker) const {
        auto mod = CitizenOfSigmar::toSaveModifier(weapon, attacker);
        if (m_shield) mod++;
        return mod;
    }

    int
    FreeguildGeneralOnGriffon::weaponRend(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Charging Lance
        if (m_charged && (weapon->name() == m_lance.name())) {
            return -2;
        }
        return CitizenOfSigmar::weaponRend(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int FreeguildGeneralOnGriffon::runModifier() const {
        // Skilled Rider
        auto mod = CitizenOfSigmar::runModifier();
        if (!m_shield) mod++;
        return mod;
    }

    int FreeguildGeneralOnGriffon::chargeModifier() const {
        // Skilled Rider
        auto mod = CitizenOfSigmar::chargeModifier();
        if (!m_shield) mod++;
        return mod;
    }

    void FreeguildGeneralOnGriffon::onRestore() {
        CitizenOfSigmar::onRestore();
        // Restore table-driven attributes
        onWounded();
    }

    void FreeguildGeneralOnGriffon::onWounded() {
        const auto damageIndex = getDamageTableIndex();
        m_claws.setAttacks(g_damageTable[damageIndex].m_clawAttacks);
        m_beak.setDamage(g_damageTable[damageIndex].m_beakDamage);
        m_move = g_damageTable[getDamageTableIndex()].m_move;

        CitizenOfSigmar::onWounded();
    }

    size_t FreeguildGeneralOnGriffon::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    int FreeguildGeneralOnGriffon::piercingBloodroar(const Unit *target) {
        // Piercing Bloodroar
        if ((target->owningPlayer() != owningPlayer()) && (distanceTo(target) <= 8.0)) {
            return -1;
        }

        return 0;
    }

    int FreeguildGeneralOnGriffon::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }


} // namespace CitiesOfSigmar