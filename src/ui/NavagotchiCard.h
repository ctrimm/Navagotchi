#pragma once

#include <lvgl.h>
#include <Arduino.h>
#include "ui/InputHandler.h"
#include "game/NavagotchiGame.h"
#include "ConfigManager.h"

/**
 * @class NavagotchiCard
 * @brief UI card for Navagotchi virtual pet game
 *
 * Features:
 * - Displays animated pet sprite
 * - Shows three stats (Hunger, Happiness, Energy) with progress bars
 * - Button cycles through actions (Feed, Play, Sleep)
 * - Persistent state across reboots
 * - Automatic stat decay over time
 */
class NavagotchiCard : public InputHandler {
public:
    /**
     * @brief Constructor
     * @param parent LVGL parent object
     * @param configManager Reference to ConfigManager for persistent storage
     */
    NavagotchiCard(lv_obj_t* parent, ConfigManager& configManager);

    /**
     * @brief Destructor - safely cleans up UI resources
     */
    ~NavagotchiCard();

    /**
     * @brief Get the underlying LVGL card object
     * @return LVGL object pointer or nullptr if not created
     */
    lv_obj_t* getCard();

    /**
     * @brief Handle button press events
     * @param button_index The index of the button that was pressed
     * @return true if center button (performs action), false otherwise
     */
    bool handleButtonPress(uint8_t button_index) override;

    /**
     * @brief Prepare card for removal
     * Called before LVGL object deletion to prevent double-deletion
     */
    void prepareForRemoval() override { _card = nullptr; }

    /**
     * @brief Update game state and UI
     * Called regularly (~60 FPS) when card is active
     * @return true to continue receiving updates
     */
    bool update() override;

private:
    /**
     * @brief Check if an LVGL object is valid
     * @param obj LVGL object to check
     * @return true if object exists and is valid
     */
    bool isValidObject(lv_obj_t* obj) const;

    /**
     * @brief Update all UI elements with current game state
     */
    void updateUI();

    /**
     * @brief Cycle to the next action (Feed → Play → Sleep → Feed)
     */
    void cycleAction();

    /**
     * @brief Perform the currently selected action
     */
    void performAction();

    /**
     * @brief Get action name as string
     * @param action The action to convert
     * @return Action name string
     */
    const char* getActionName(NavagotchiGame::Action action) const;

    /**
     * @brief Get action emoji
     * @param action The action to get emoji for
     * @return Action emoji string
     */
    const char* getActionEmoji(NavagotchiGame::Action action) const;

    // Game logic
    NavagotchiGame* game;  ///< Game logic instance

    // UI Elements
    lv_obj_t* _card;           ///< Main container
    lv_obj_t* _background;     ///< Background container
    lv_obj_t* _anim_img;       ///< Pet sprite animation
    lv_obj_t* _title_label;    ///< Title "NAVAGOTCHI"
    lv_obj_t* _hunger_label;   ///< Hunger stat label
    lv_obj_t* _happy_label;    ///< Happiness stat label
    lv_obj_t* _energy_label;   ///< Energy stat label
    lv_obj_t* _hunger_bar;     ///< Hunger progress bar
    lv_obj_t* _happy_bar;      ///< Happiness progress bar
    lv_obj_t* _energy_bar;     ///< Energy progress bar
    lv_obj_t* _action_label;   ///< Current action indicator

    // State
    NavagotchiGame::Action currentAction;  ///< Currently selected action
    unsigned long lastUIUpdate;            ///< Last UI update timestamp
    static constexpr unsigned long UI_UPDATE_INTERVAL_MS = 1000;  ///< Update UI every second
};
