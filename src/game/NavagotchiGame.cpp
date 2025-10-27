#include "game/NavagotchiGame.h"

NavagotchiGame::NavagotchiGame(ConfigManager& configManager)
    : configManager(configManager)
    , hunger(50)
    , happiness(50)
    , energy(50)
    , lastDecayTime(millis()) {

    // Load saved state if available
    loadState();
}

NavagotchiGame::~NavagotchiGame() {
    // Save state when game is destroyed
    saveState();
}

void NavagotchiGame::update() {
    unsigned long currentTime = millis();

    // Check if enough time has passed for stat decay
    if (currentTime - lastDecayTime >= DECAY_INTERVAL_MS) {
        // Increase hunger (pet gets hungrier)
        hunger = clamp(hunger + DECAY_AMOUNT, 0, 100);

        // Decrease happiness
        happiness = clamp(happiness - DECAY_AMOUNT, 0, 100);

        // Decrease energy
        energy = clamp(energy - DECAY_AMOUNT, 0, 100);

        // Update last decay time
        lastDecayTime = currentTime;

        // Save state after decay
        saveState();
    }
}

void NavagotchiGame::feed() {
    // Reduce hunger by 20
    hunger = clamp(hunger - 20, 0, 100);

    // Feeding also increases happiness slightly
    happiness = clamp(happiness + 5, 0, 100);

    saveState();
}

void NavagotchiGame::play() {
    // Increase happiness by 20
    happiness = clamp(happiness + 20, 0, 100);

    // Playing uses energy
    energy = clamp(energy - 5, 0, 100);

    // Playing also makes pet slightly hungry
    hunger = clamp(hunger + 5, 0, 100);

    saveState();
}

void NavagotchiGame::sleep() {
    // Increase energy by 20
    energy = clamp(energy + 20, 0, 100);

    // Sleeping makes pet slightly hungry
    hunger = clamp(hunger + 5, 0, 100);

    saveState();
}

NavagotchiGame::Mood NavagotchiGame::getMood() const {
    // Critical if any stat is very low
    if (hunger >= 90 || happiness <= 10 || energy <= 10) {
        return Mood::CRITICAL;
    }

    // Sad if any stat is low
    if (hunger >= 70 || happiness <= 30 || energy <= 30) {
        return Mood::SAD;
    }

    // Happy if all stats are good
    if (hunger <= 30 && happiness >= 70 && energy >= 70) {
        return Mood::HAPPY;
    }

    // Otherwise okay
    return Mood::OKAY;
}

float NavagotchiGame::getAnimationSpeed() const {
    Mood mood = getMood();

    switch (mood) {
        case Mood::HAPPY:
            return 0.7f;  // Faster animation when happy
        case Mood::OKAY:
            return 1.0f;  // Normal speed
        case Mood::SAD:
            return 1.5f;  // Slower when sad
        case Mood::CRITICAL:
            return 2.0f;  // Very slow when critical
        default:
            return 1.0f;
    }
}

void NavagotchiGame::loadState() {
    // Load stats from preferences
    hunger = configManager.getNavagotchiHunger();
    happiness = configManager.getNavagotchiHappiness();
    energy = configManager.getNavagotchiEnergy();

    // Clamp values in case of corrupted data
    hunger = clamp(hunger, 0, 100);
    happiness = clamp(happiness, 0, 100);
    energy = clamp(energy, 0, 100);

    Serial.printf("Navagotchi: Loaded state - Hunger: %d, Happiness: %d, Energy: %d\n",
                  hunger, happiness, energy);
}

void NavagotchiGame::saveState() {
    configManager.setNavagotchiState(hunger, happiness, energy);

    Serial.printf("Navagotchi: Saved state - Hunger: %d, Happiness: %d, Energy: %d\n",
                  hunger, happiness, energy);
}

int NavagotchiGame::clamp(int value, int min, int max) const {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}
