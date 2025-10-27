#pragma once

#include <Arduino.h>
#include "ConfigManager.h"

/**
 * @class NavagotchiGame
 * @brief Virtual pet game with three stats: Hunger, Happiness, Energy
 *
 * Features:
 * - Three stats that decay over time (Hunger, Happiness, Energy)
 * - Three actions: Feed (reduces hunger), Play (increases happiness), Sleep (increases energy)
 * - Persistent state saved to ConfigManager
 * - Mood calculation based on overall stat health
 */
class NavagotchiGame {
public:
    /**
     * @brief Mood states based on overall pet health
     */
    enum class Mood {
        HAPPY,      ///< All stats > 60
        OKAY,       ///< All stats > 30
        SAD,        ///< Any stat <= 30
        CRITICAL    ///< Any stat <= 10
    };

    /**
     * @brief Available actions the user can perform
     */
    enum class Action {
        FEED = 0,
        PLAY = 1,
        SLEEP = 2
    };

    /**
     * @brief Constructor
     * @param configManager Reference to ConfigManager for persistent storage
     */
    NavagotchiGame(ConfigManager& configManager);

    /**
     * @brief Destructor - saves state
     */
    ~NavagotchiGame();

    /**
     * @brief Update game state (call regularly from update() loop)
     *
     * Handles stat decay over time. Stats decrease by 1 every 30 seconds.
     */
    void update();

    /**
     * @brief Perform the Feed action
     *
     * Decreases hunger by 20 (clamped to 0)
     */
    void feed();

    /**
     * @brief Perform the Play action
     *
     * Increases happiness by 20 (clamped to 100)
     */
    void play();

    /**
     * @brief Perform the Sleep action
     *
     * Increases energy by 20 (clamped to 100)
     */
    void sleep();

    /**
     * @brief Get current hunger level (0-100, 0 = full, 100 = starving)
     */
    int getHunger() const { return hunger; }

    /**
     * @brief Get current happiness level (0-100, 0 = sad, 100 = happy)
     */
    int getHappiness() const { return happiness; }

    /**
     * @brief Get current energy level (0-100, 0 = exhausted, 100 = energized)
     */
    int getEnergy() const { return energy; }

    /**
     * @brief Get current mood based on stats
     */
    Mood getMood() const;

    /**
     * @brief Get animation speed multiplier based on mood
     *
     * @return Speed multiplier (1.0 = normal, 2.0 = fast when happy, 0.5 = slow when sad)
     */
    float getAnimationSpeed() const;

private:
    ConfigManager& configManager;

    // Pet stats (0-100 range)
    int hunger;      ///< 0 = full, 100 = starving
    int happiness;   ///< 0 = sad, 100 = happy
    int energy;      ///< 0 = exhausted, 100 = energized

    // Timing for stat decay
    unsigned long lastDecayTime;  ///< Timestamp of last stat decay
    static constexpr unsigned long DECAY_INTERVAL_MS = 30000;  ///< Decay every 30 seconds
    static constexpr int DECAY_AMOUNT = 5;  ///< Amount to decay each interval

    /**
     * @brief Load saved state from ConfigManager
     */
    void loadState();

    /**
     * @brief Save current state to ConfigManager
     */
    void saveState();

    /**
     * @brief Clamp a value between min and max
     */
    int clamp(int value, int min, int max) const;
};
