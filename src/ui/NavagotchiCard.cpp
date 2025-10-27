#include "ui/NavagotchiCard.h"
#include "Style.h"
#include "sprites/sprites.h"
#include "hardware/Input.h"

NavagotchiCard::NavagotchiCard(lv_obj_t* parent, ConfigManager& configManager)
    : game(nullptr)
    , _card(nullptr)
    , _background(nullptr)
    , _anim_img(nullptr)
    , _title_label(nullptr)
    , _hunger_label(nullptr)
    , _happy_label(nullptr)
    , _energy_label(nullptr)
    , _hunger_bar(nullptr)
    , _happy_bar(nullptr)
    , _energy_bar(nullptr)
    , _action_label(nullptr)
    , currentAction(NavagotchiGame::Action::FEED)
    , lastUIUpdate(0) {

    // Create game instance
    game = new NavagotchiGame(configManager);

    // Create main card with black background
    _card = lv_obj_create(parent);
    if (!_card) return;

    lv_obj_set_width(_card, lv_pct(100));
    lv_obj_set_height(_card, lv_pct(100));
    lv_obj_set_style_bg_color(_card, lv_color_black(), 0);
    lv_obj_set_style_border_width(_card, 0, 0);
    lv_obj_set_style_pad_all(_card, 5, 0);

    // Create green container
    _background = lv_obj_create(_card);
    if (!_background) return;

    lv_obj_set_style_radius(_background, 8, LV_PART_MAIN);
    lv_obj_set_style_bg_color(_background, lv_color_hex(0x0E7A00), 0);
    lv_obj_set_style_border_width(_background, 0, 0);
    lv_obj_set_style_pad_all(_background, 8, 0);
    lv_obj_set_width(_background, lv_pct(100));
    lv_obj_set_height(_background, lv_pct(100));

    // Create title label
    _title_label = lv_label_create(_background);
    if (_title_label) {
        lv_obj_set_style_text_font(_title_label, Style::loudNoisesFont(), 0);
        lv_obj_set_style_text_color(_title_label, lv_color_white(), 0);
        lv_label_set_text(_title_label, "NAVAGOTCHI");
        lv_obj_align(_title_label, LV_ALIGN_TOP_MID, 0, 2);
    }

    // Create animation image (pet sprite)
    _anim_img = lv_animimg_create(_background);
    if (_anim_img) {
        lv_animimg_set_src(_anim_img, (const void**)walking_sprites, walking_sprites_count);
        lv_animimg_set_duration(_anim_img, 1000);  // Will be adjusted by mood
        lv_animimg_set_repeat_count(_anim_img, LV_ANIM_REPEAT_INFINITE);
        lv_img_set_zoom(_anim_img, 512);  // 2x size
        lv_obj_align(_anim_img, LV_ALIGN_LEFT_MID, 5, 0);
        lv_animimg_start(_anim_img);
    }

    // Create stat labels (right side)
    int labelX = 80;
    int labelStartY = 20;
    int labelSpacing = 25;

    _hunger_label = lv_label_create(_background);
    if (_hunger_label) {
        lv_obj_set_style_text_color(_hunger_label, lv_color_white(), 0);
        lv_label_set_text(_hunger_label, "HUNGER: 0");
        lv_obj_set_pos(_hunger_label, labelX, labelStartY);
    }

    _happy_label = lv_label_create(_background);
    if (_happy_label) {
        lv_obj_set_style_text_color(_happy_label, lv_color_white(), 0);
        lv_label_set_text(_happy_label, "HAPPY: 0");
        lv_obj_set_pos(_happy_label, labelX, labelStartY + labelSpacing);
    }

    _energy_label = lv_label_create(_background);
    if (_energy_label) {
        lv_obj_set_style_text_color(_energy_label, lv_color_white(), 0);
        lv_label_set_text(_energy_label, "ENERGY: 0");
        lv_obj_set_pos(_energy_label, labelX, labelStartY + labelSpacing * 2);
    }

    // Create progress bars
    int barX = labelX;
    int barY = labelStartY + 13;
    int barWidth = 140;
    int barHeight = 8;

    _hunger_bar = lv_bar_create(_background);
    if (_hunger_bar) {
        lv_obj_set_size(_hunger_bar, barWidth, barHeight);
        lv_obj_set_pos(_hunger_bar, barX, barY);
        lv_bar_set_range(_hunger_bar, 0, 100);
        lv_bar_set_value(_hunger_bar, 0, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(_hunger_bar, lv_color_hex(0x333333), LV_PART_MAIN);
        lv_obj_set_style_bg_color(_hunger_bar, lv_color_hex(0xFF4444), LV_PART_INDICATOR);
    }

    _happy_bar = lv_bar_create(_background);
    if (_happy_bar) {
        lv_obj_set_size(_happy_bar, barWidth, barHeight);
        lv_obj_set_pos(_happy_bar, barX, barY + labelSpacing);
        lv_bar_set_range(_happy_bar, 0, 100);
        lv_bar_set_value(_happy_bar, 0, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(_happy_bar, lv_color_hex(0x333333), LV_PART_MAIN);
        lv_obj_set_style_bg_color(_happy_bar, lv_color_hex(0xFFDD44), LV_PART_INDICATOR);
    }

    _energy_bar = lv_bar_create(_background);
    if (_energy_bar) {
        lv_obj_set_size(_energy_bar, barWidth, barHeight);
        lv_obj_set_pos(_energy_bar, barX, barY + labelSpacing * 2);
        lv_bar_set_range(_energy_bar, 0, 100);
        lv_bar_set_value(_energy_bar, 0, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(_energy_bar, lv_color_hex(0x333333), LV_PART_MAIN);
        lv_obj_set_style_bg_color(_energy_bar, lv_color_hex(0x44DDFF), LV_PART_INDICATOR);
    }

    // Create action label at bottom
    _action_label = lv_label_create(_background);
    if (_action_label) {
        lv_obj_set_style_text_font(_action_label, Style::loudNoisesFont(), 0);
        lv_obj_set_style_text_color(_action_label, lv_color_white(), 0);
        lv_obj_align(_action_label, LV_ALIGN_BOTTOM_MID, 0, -5);
    }

    // Initial UI update
    updateUI();
}

NavagotchiCard::~NavagotchiCard() {
    // Clean up game
    if (game) {
        delete game;
        game = nullptr;
    }

    // Clean up UI elements
    if (isValidObject(_card)) {
        lv_obj_add_flag(_card, LV_OBJ_FLAG_HIDDEN);
        lv_obj_del_async(_card);

        // When card is deleted, all children are automatically deleted
        _card = nullptr;
        _background = nullptr;
        _anim_img = nullptr;
        _title_label = nullptr;
        _hunger_label = nullptr;
        _happy_label = nullptr;
        _energy_label = nullptr;
        _hunger_bar = nullptr;
        _happy_bar = nullptr;
        _energy_bar = nullptr;
        _action_label = nullptr;
    }
}

lv_obj_t* NavagotchiCard::getCard() {
    return _card;
}

bool NavagotchiCard::handleButtonPress(uint8_t button_index) {
    if (button_index == Input::BUTTON_CENTER) {
        // Perform current action
        performAction();
        // Cycle to next action
        cycleAction();
        return true;  // We handled this button press
    }

    return false;  // Allow navigation
}

bool NavagotchiCard::update() {
    if (!game) return false;

    // Update game state (handles stat decay)
    game->update();

    // Update UI periodically (not every frame)
    unsigned long currentTime = millis();
    if (currentTime - lastUIUpdate >= UI_UPDATE_INTERVAL_MS) {
        updateUI();
        lastUIUpdate = currentTime;
    }

    return true;  // Continue receiving updates
}

void NavagotchiCard::updateUI() {
    if (!game) return;

    // Get current stats
    int hunger = game->getHunger();
    int happiness = game->getHappiness();
    int energy = game->getEnergy();

    // Update stat labels
    if (isValidObject(_hunger_label)) {
        char buf[32];
        snprintf(buf, sizeof(buf), "HUNGER: %d", hunger);
        lv_label_set_text(_hunger_label, buf);
    }

    if (isValidObject(_happy_label)) {
        char buf[32];
        snprintf(buf, sizeof(buf), "HAPPY: %d", happiness);
        lv_label_set_text(_happy_label, buf);
    }

    if (isValidObject(_energy_label)) {
        char buf[32];
        snprintf(buf, sizeof(buf), "ENERGY: %d", energy);
        lv_label_set_text(_energy_label, buf);
    }

    // Update progress bars (inverted for hunger - higher value = worse)
    if (isValidObject(_hunger_bar)) {
        lv_bar_set_value(_hunger_bar, 100 - hunger, LV_ANIM_ON);
    }

    if (isValidObject(_happy_bar)) {
        lv_bar_set_value(_happy_bar, happiness, LV_ANIM_ON);
    }

    if (isValidObject(_energy_bar)) {
        lv_bar_set_value(_energy_bar, energy, LV_ANIM_ON);
    }

    // Update animation speed based on mood
    if (isValidObject(_anim_img)) {
        float speedMultiplier = game->getAnimationSpeed();
        int duration = (int)(1000 * speedMultiplier);
        lv_animimg_set_duration(_anim_img, duration);
    }

    // Update action label
    if (isValidObject(_action_label)) {
        char buf[64];
        snprintf(buf, sizeof(buf), "%s [%s]", getActionEmoji(currentAction), getActionName(currentAction));
        lv_label_set_text(_action_label, buf);
    }
}

void NavagotchiCard::cycleAction() {
    // Cycle through actions: FEED -> PLAY -> SLEEP -> FEED
    switch (currentAction) {
        case NavagotchiGame::Action::FEED:
            currentAction = NavagotchiGame::Action::PLAY;
            break;
        case NavagotchiGame::Action::PLAY:
            currentAction = NavagotchiGame::Action::SLEEP;
            break;
        case NavagotchiGame::Action::SLEEP:
            currentAction = NavagotchiGame::Action::FEED;
            break;
    }

    // Update UI to show new action
    updateUI();
}

void NavagotchiCard::performAction() {
    if (!game) return;

    // Perform the selected action
    switch (currentAction) {
        case NavagotchiGame::Action::FEED:
            game->feed();
            Serial.println("Navagotchi: Fed pet!");
            break;
        case NavagotchiGame::Action::PLAY:
            game->play();
            Serial.println("Navagotchi: Played with pet!");
            break;
        case NavagotchiGame::Action::SLEEP:
            game->sleep();
            Serial.println("Navagotchi: Pet is sleeping!");
            break;
    }

    // Immediate UI update after action
    updateUI();
}

const char* NavagotchiCard::getActionName(NavagotchiGame::Action action) const {
    switch (action) {
        case NavagotchiGame::Action::FEED:
            return "FEED";
        case NavagotchiGame::Action::PLAY:
            return "PLAY";
        case NavagotchiGame::Action::SLEEP:
            return "SLEEP";
        default:
            return "???";
    }
}

const char* NavagotchiCard::getActionEmoji(NavagotchiGame::Action action) const {
    switch (action) {
        case NavagotchiGame::Action::FEED:
            return "PRESS";
        case NavagotchiGame::Action::PLAY:
            return "PRESS";
        case NavagotchiGame::Action::SLEEP:
            return "PRESS";
        default:
            return "?";
    }
}

bool NavagotchiCard::isValidObject(lv_obj_t* obj) const {
    return obj && lv_obj_is_valid(obj);
}
