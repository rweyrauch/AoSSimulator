/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <gtkmm.h>
#include <map>
#include <string>
#include <Board.h>
#include <Dice.h>
#include <ManoAMano.h>
#include <UnitFactory.h>
#include "ResultsDisplay.h"
#include <iostream>

static Gtk::Window* pWindow = nullptr;
static ResultsDisplay* pDialog = nullptr;

static Gtk::SpinButton* pNumRounds = nullptr;
static Gtk::Entry* pNumIterations = nullptr;

static Gtk::ComboBoxText *pRedAlliance = nullptr;
static Gtk::ComboBoxText *pBlueAlliance = nullptr;
static Gtk::ComboBoxText *pRedFaction = nullptr;
static Gtk::ComboBoxText *pBlueFaction = nullptr;
static Gtk::ComboBoxText *pRedUnits = nullptr;
static Gtk::ComboBoxText *pBlueUnits = nullptr;
static Gtk::Box *pRedUnitConfig = nullptr;
static Gtk::Box *pBlueUnitConfig = nullptr;
static Gtk::CheckButton *pVerbose = nullptr;
static Gtk::CheckButton *pSaveMaps = nullptr;

static int g_numRounds = 5;
static Verbosity g_verboseLevel = Verbosity::Silence;
static int g_numIterations = 1;
static bool g_saveMaps = false;
static int g_redAlliance = ORDER;
static int g_blueAlliance = ORDER;

static ManoAMano* g_battle = nullptr;
static Unit* g_pRed = nullptr;
static Unit* g_pBlue = nullptr;

static void runSimulation();

void createConfigUI(const std::string& unitName, Gtk::Box *pContainer);
Unit* createUnit(const std::string& unitName, Gtk::Box* pUnitUI);

static void on_quit_clicked()
{
    if (pWindow)
        pWindow->hide();
}

static void on_start_clicked()
{
    g_numRounds = pNumRounds->get_value_as_int();
    g_numIterations = (int)std::strtol(pNumIterations->get_text().c_str(), nullptr, 10);
    if (pVerbose->get_active())
    {
        g_verboseLevel = Verbosity::Narrative;
    }
    SetVerbosity(g_verboseLevel);
    g_saveMaps = pSaveMaps->get_active();
    g_pRed = createUnit(pRedUnits->get_active_text(), pRedUnitConfig);
    g_pBlue = createUnit(pBlueUnits->get_active_text(), pBlueUnitConfig);
    runSimulation();
}

static void populateUnits(const std::string& factionName, Gtk::ComboBoxText *pCombo)
{
    pCombo->remove_all();
    std::list<std::string> unitNames;

    for (auto ruip = UnitFactory::RegisteredUnitsBegin(); ruip != UnitFactory::RegisteredUnitsEnd(); ++ruip)
    {
        auto ki = FactionStringToKeyword(factionName);
        // filter based on keyword
        for (auto fip : ruip->second.m_factions)
        {
            if (ki == fip)
                unitNames.push_back(ruip->first);
        }
    }
    if (!unitNames.empty())
    {
        unitNames.sort();
        unitNames.unique();
        for (const auto &ip : unitNames)
        {
            pCombo->append(ip);
        }
        pCombo->set_active(0);
    }
}

static void populateFactions(int allianceId, Gtk::ComboBoxText *pCombo)
{
    pCombo->remove_all();
    std::list<Keyword> factionKeywords;
    for (auto ruip = UnitFactory::RegisteredUnitsBegin(); ruip != UnitFactory::RegisteredUnitsEnd(); ++ruip)
    {
        if (allianceId == ruip->second.m_grandAlliance)
        {
            for (auto fip : ruip->second.m_factions)
            {
                factionKeywords.push_back(fip);
            }
        }
    }
    factionKeywords.sort();
    factionKeywords.unique();
    for (auto ip : factionKeywords)
    {
        pCombo->append(FactionKeywordToString(ip));
    }
    pCombo->set_active(0);
}

static void on_red_alliance_selected()
{
    if (pRedAlliance)
    {
        auto alliance = pRedAlliance->get_active_text();
        g_redAlliance = GrandAllianceStringToKeyword(alliance);

        std::cout << "Red selected Grand Alliance " << alliance << " with ID: " << g_redAlliance << std::endl;

        populateFactions(g_redAlliance, pRedFaction);
    }
}

static void on_blue_alliance_selected()
{
    if (pBlueAlliance)
    {
        auto alliance = pBlueAlliance->get_active_text();
        g_blueAlliance = GrandAllianceStringToKeyword(alliance);

        std::cout << "Blue selected Grand Alliance " << alliance << " with ID: " << g_blueAlliance << std::endl;

        populateFactions(g_blueAlliance, pBlueFaction);
    }
}

static void on_red_faction_selected()
{
    if (pRedFaction && pRedUnits)
    {
        auto factionName = pRedFaction->get_active_text();
        populateUnits(factionName, pRedUnits);
    }
}

static void on_blue_faction_selected()
{
    if (pBlueFaction && pBlueUnits)
    {
        auto factionName = pBlueFaction->get_active_text();
        populateUnits(factionName, pBlueUnits);
    }
}

static void on_red_unit_select()
{
    if (pRedUnits && pRedUnitConfig)
    {
        auto unitName = pRedUnits->get_active_text();
        createConfigUI(unitName, pRedUnitConfig);
    }
}

static void on_blue_unit_select()
{
    if (pBlueUnits && pBlueUnitConfig)
    {
        auto unitName = pBlueUnits->get_active_text();
        createConfigUI(unitName, pBlueUnitConfig);
    }
}

static void runSimulation()
{
    if (g_pRed == nullptr || g_pBlue == nullptr)
    {
        std::cerr << "Player units not created." << std::endl;
        return;
    }

    std::string mapBaseName("battlemap");

    auto board = Board::Instance();

    g_battle->combatants(g_pRed, g_pBlue);

    std::cout << "Red Points: " << g_pRed->points() << "   Blue Points: " << g_pBlue->points() << std::endl;

    std::stringstream fn;

    int redVictories = 0;
    int blueVictories = 0;
    int ties = 0;

    for (auto i = 0; i < g_numIterations; i++)
    {
        g_pRed->restore();
        g_pBlue->restore();

        g_battle->start();

        if (g_saveMaps)
        {
            fn.str("");
            fn << mapBaseName << "_start.png";
            board->render(fn.str());
        }

        while (!g_battle->done())
        {
            g_battle->simulate();

            auto round = g_battle->currentRound();

            if (g_saveMaps)
            {
                fn.str("");
                fn << mapBaseName << "_round_" << round << ".png";
                board->render(fn.str());
            }

            g_battle->next();
        }

        auto victor = g_battle->getVictor();

        //if (g_verboseLevel > 0)
        {
            std::cout << "Team " << PlayerIdToString(victor) << " was victorious." << std::endl;
            g_battle->logStatistics();
        }

        if (victor == PlayerId::Blue)
            blueVictories++;
        else if (victor == PlayerId::Red)
            redVictories++;
        else
            ties++;

        if (g_saveMaps)
        {
            fn.str("");
            fn << mapBaseName << "_end.png";
            board->render(fn.str());
        }
    }

    const float redPercent = (float)redVictories * 100.0f/g_numIterations;
    const float bluePercent = (float)blueVictories * 100.0f/g_numIterations;
    const float tiesPercent = (float)ties * 100.0f/g_numIterations;

    std::cout << "Victor Breakdown (%):" << std::endl
              << "\tRed: " << redPercent << std::endl
              << "\tBlue: " << bluePercent << std::endl
              << "\tTies: " << tiesPercent << std::endl;

    pDialog->setResults(g_pRed->points(), g_pBlue->points(), g_numRounds, g_numIterations,
        redPercent, bluePercent, tiesPercent);
}

int main(int argc, char *argv[])
{
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.examples.base");

    Initialize(Verbosity::Normal);

    g_battle = new ManoAMano(g_numRounds);

    // Construct the top-level UI from the glade description.
    auto builder = Gtk::Builder::create_from_file("manoamano.glade");

    // Find the  top-level window.
    builder->get_widget("manoamanoWindow", pWindow);

    // Hook up callbacks
    Gtk::Button* pQuitButton = nullptr;
    builder->get_widget("quitButton", pQuitButton);
    pQuitButton->signal_clicked().connect(sigc::ptr_fun(on_quit_clicked));

    Gtk::Button* pStartButton = nullptr;
    builder->get_widget("startButton", pStartButton);
    pStartButton->signal_clicked().connect(sigc::ptr_fun(on_start_clicked));

    builder->get_widget("numberOfRounds", pNumRounds);
    pNumRounds->set_numeric();
    pNumRounds->set_digits(0);
    pNumRounds->set_range(1.0, 10.0);
    pNumRounds->set_increments(1.0, 1.0);
    pNumRounds->set_value(5);

    builder->get_widget("numberOfIterations", pNumIterations);
    pNumIterations->set_text("10");

    builder->get_widget("redGrandAlliance", pRedAlliance);
    builder->get_widget("blueGrandAlliance", pBlueAlliance);

    pRedAlliance->signal_changed().connect(sigc::ptr_fun(on_red_alliance_selected));
    pBlueAlliance->signal_changed().connect(sigc::ptr_fun(on_blue_alliance_selected));

    pRedAlliance->set_active(0);
    pBlueAlliance->set_active(0);

    // Add factions
    builder->get_widget("redFaction", pRedFaction);
    builder->get_widget("blueFaction", pBlueFaction);

    pRedFaction->signal_changed().connect(sigc::ptr_fun(on_red_faction_selected));
    pBlueFaction->signal_changed().connect(sigc::ptr_fun(on_blue_faction_selected));

    populateFactions(0, pRedFaction);
    pRedFaction->set_active(0);
    populateFactions(0, pBlueFaction);
    pBlueFaction->set_active(0);

    builder->get_widget("redUnits", pRedUnits);
    builder->get_widget("blueUnits", pBlueUnits);

    pRedUnits->signal_changed().connect(sigc::ptr_fun(on_red_unit_select));
    pBlueUnits->signal_changed().connect(sigc::ptr_fun(on_blue_unit_select));

    auto factionName = pRedFaction->get_active_text();
    if (!factionName.empty())
        populateUnits(factionName, pRedUnits);

    factionName = pBlueFaction->get_active_text();
    if (!factionName.empty())
        populateUnits(factionName, pBlueUnits);

    builder->get_widget("redUnitConfig", pRedUnitConfig);
    builder->get_widget("blueUnitConfig", pBlueUnitConfig);

    createConfigUI(pRedUnits->get_active_text(), pRedUnitConfig);
    createConfigUI(pBlueUnits->get_active_text(), pBlueUnitConfig);

    builder->get_widget("verboseEnabled", pVerbose);
    builder->get_widget("saveMaps", pSaveMaps);

    pDialog = new ResultsDisplay();
    pDialog->build(builder);

    return app->run(*pWindow);
}

void createConfigUI(const std::string& unitName, Gtk::Box *pContainer)
{
    auto factory = UnitFactory::LookupUnit(unitName);
    if (factory)
    {
        auto previousWidgets = pContainer->get_children();
        if (!previousWidgets.empty())
        {
            for (auto ip : previousWidgets)
            {
                pContainer->remove(*ip);
                delete ip;
            }
        }

        auto pGrid = new Gtk::Grid();
        pGrid->set_row_spacing(4);
        pGrid->set_column_spacing(4);

        int left = 0;
        int right = 1;
        int top = 0;
        std::vector<Parameter> defaultParams = factory->m_parameters;
        for (const auto& ip : defaultParams)
        {
            if (ip.paramType == ParamType::Integer)
            {
                Gtk::Label* pLabel = Gtk::manage(new Gtk::Label());
                pLabel->set_text(ip.name);
                pGrid->attach(*pLabel, left, top, 1, 1);
                pLabel->show();

                Gtk::SpinButton *pSpin = Gtk::manage(new Gtk::SpinButton());
                pSpin->set_numeric();
                pSpin->set_digits(0);
                pSpin->set_range(ip.minValue, ip.maxValue);
                pSpin->set_increments(ip.increment, ip.increment);
                pSpin->set_value(ip.intValue);
                pGrid->attach(*pSpin, right, top, 1, 1);
                pSpin->show();
                top++;
            }
            else if (ip.paramType == ParamType::Enum)
            {
                Gtk::Label* pLabel = Gtk::manage(new Gtk::Label());
                pLabel->set_text(ip.name);
                pGrid->attach(*pLabel, left, top, 1, 1);
                pLabel->show();

                Gtk::ComboBoxText *pCombo = Gtk::manage(new Gtk::ComboBoxText());
                for (auto i = 0; i < ip.numValues; i++)
                {
                    Parameter param = ip;
                    param.intValue = ip.values[i];
                    pCombo->append(factory->m_paramToString(param));
                }
                pCombo->set_active_text(factory->m_paramToString(ip));
                pGrid->attach(*pCombo, right, top, 1, 1);
                pCombo->show();
                top++;
            }
            else if (ip.paramType == ParamType::Boolean)
            {
                Gtk::Label* pLabel = Gtk::manage(new Gtk::Label());
                pLabel->set_text(ip.name);
                pGrid->attach(*pLabel, left, top, 1, 1);
                pLabel->show();
                Gtk::CheckButton* pCheck = Gtk::manage(new Gtk::CheckButton());
                pCheck->set_active((ip.intValue == 0) ? false : true);
                pGrid->attach(*pCheck, right, top, 1, 1);
                pCheck->show();
                top++;
            }
        }
        pContainer->pack_start(*pGrid);
        pGrid->show();
    }
}

Unit* createUnit(const std::string& unitName, Gtk::Box* pUnitUI)
{
    auto factory = UnitFactory::LookupUnit(unitName);

    auto parameters = factory->m_parameters;

    // extract parameters from UI
    if (pUnitUI)
    {
        auto children = pUnitUI->get_children();
        for (auto ip : children)
        {
            Gtk::Widget* pWidget = ip;
            auto pGrid = dynamic_cast<Gtk::Grid*>(pWidget);
            if (pGrid)
            {
                auto paramWidgets = pGrid->get_children();
                //std::cout << "Found the grid." << "  Num param widgets: " << paramWidgets.size() << std::endl;
                auto numRows = paramWidgets.size() / 2; // We _know_ that there are only 2 columns.
                for (auto r = 0; r < numRows; r++)
                {
                    auto label = dynamic_cast<Gtk::Label*>(pGrid->get_child_at(0, r));
                    auto value = pGrid->get_child_at(1, r);
                    //std::cout << "Row: " << r << "  Label: " << label->get_text() << std::endl;
                    ParameterList::iterator pp = FindParam(label->get_text(), parameters);
                    if (pp != parameters.end())
                    {
                        //std::cout << "Found matching parameter for " << pp->m_name << " of type " << (int)pp->m_paramType << std::endl;
                        if (pp->paramType == ParamType::Integer)
                        {
                            auto pSpin = dynamic_cast<Gtk::SpinButton*>(value);
                            if (pSpin)
                            {
                                pp->intValue = pSpin->get_value_as_int();
                            }
                        }
                        else if (pp->paramType == ParamType::Boolean)
                        {
                            auto pCheck = dynamic_cast<Gtk::CheckButton*>(value);
                            if (pCheck)
                            {
                                pp->intValue = pCheck->get_active() ? 1 : 0;
                            }
                        }
                        else if (pp->paramType == ParamType::Enum)
                        {
                            auto pCombo = dynamic_cast<Gtk::ComboBoxText*>(value);
                            if (pCombo)
                            {
                                pp->intValue = factory->m_enumStringToInt(pCombo->get_active_text());
                            }
                        }
                    }
                }
            }
        }
    }

    return UnitFactory::Create(unitName, parameters);
}
