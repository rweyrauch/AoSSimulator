/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <gloomspitegitz/Zarbag.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>

namespace GloomspiteGitz {
    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 80;

    bool Zarbag::s_registered = false;

    Zarbag::Zarbag() :
            GloomspiteGitzBase("Zarbag", 5, WOUNDS, 4, 6, false),
            m_sickle(Weapon::Type::Melee, "Cursed Sickle", 2, 3, 3, 3, -1, 1) {
        m_keywords = {DESTRUCTION, GROT, GLOOMSPITE_GITZ, MOONCLAN, HERO, WIZARD, MADCAP_SHAMAN, ZARBAG};
        m_weapons = {&m_sickle};

        m_totalUnbinds = 1;
        m_totalSpells = 1;
    }

    bool Zarbag::configure(Lore lore) {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_sickle);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
        //m_knownSpells.push_back(std::make_unique<FaceOfDaBadMoon>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));

        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *Zarbag::Create(const ParameterList &parameters) {
        auto unit = new Zarbag();
        auto lore = (Lore) GetEnumParam("Lore of the Moonclans", parameters, (int)None);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Zarbag::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Zarbag::Create,
                    Zarbag::ValueToString,
                    Zarbag::EnumStringToInt,
                    Zarbag::ComputePoints,
                    {
                            EnumParameter("Lore of the Moonclans", None, g_loreOfTheMoonclans)
                    },
                    DESTRUCTION,
                    {GLOOMSPITE_GITZ}
            };
            s_registered = UnitFactory::Register("Zarbag", factoryMethod);
        }
    }

    std::string Zarbag::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Lore of the Moonclans") {
            return ToString((Lore) parameter.intValue);
        }
        return ParameterValueToString(parameter);
    }

    int Zarbag::EnumStringToInt(const std::string &enumString) {
        Lore lore;
        if (FromString(enumString, lore)) {
            return (int) lore;
        }
        return 0;
    }

    int Zarbag::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace GloomspiteGitz
