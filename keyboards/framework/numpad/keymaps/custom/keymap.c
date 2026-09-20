#include QMK_KEYBOARD_H

enum combos {
    COMBO_P0_PPLS,  // Hold P0 + P+ to toggle backlight brightness
    COMBO_P0_PENT,  // Hold P0 + PEnter to cycle through backlight levels
    COMBO_LENGTH
};

const uint16_t PROGMEM p0_ppls_combo[] = {KC_P0, KC_PPLS, COMBO_END};
const uint16_t PROGMEM p0_pent_combo[] = {KC_P0, KC_PENT, COMBO_END};

combo_t key_combos[] = {
    [COMBO_P0_PPLS] = COMBO(p0_ppls_combo, BL_BRTG),
    [COMBO_P0_PENT] = COMBO(p0_pent_combo, BL_STEP),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
     /*
     *         ┌────┬────┬────┬────┐
     *  4 keys │Esc │Calc│ =  │ <- │
     *         ├────┼────┼────┼────┤
     *  4 keys │ Num│ /  │ *  │ -  │
     *         ├────┼────┼────┼────┤
     *  3 keys │ 7  │ 8  │ 9  │    │
     *         ├────┼────┼────┤    │
     *  4 keys │ 4  │ 5  │ 6  │ +  │
     *         ├────┼────┼────┼────┤
     *  3 keys │ 1  │ 2  │ 3  │    │
     *         ├────┴────┼────┤    │
     *  3 keys │ 0       │ .  │Entr│
     *         └─────────┴────┴────┴
     * 21 total
     */
    [_NUMLOCK] = LAYOUT(
        KC_ESC,  KC_CALC, KC_EQL,  KC_BSPC,
        KC_NUM,  KC_PSLS, KC_PAST, KC_PMNS,
        KC_P7,   KC_P8,   KC_P9,
        KC_P4,   KC_P5,   KC_P6,   KC_PPLS,
        KC_P1,   KC_P2,   KC_P3,
            KC_P0,        KC_PDOT, KC_PENT
    ),
     /*
     * Extra keys for when numlock is disabled.
     * Numlock keys are passed through to the number layer,
     * and automatically remapped by the OS.
     *         ┌────┬────┬────┬────┐
     *  4 keys │    │Comp│    │    │
     *         ├────┼────┼────┼────┤
     *  4 keys │    │    │    │    │
     *         ├────┼────┼────┼────┤
     *  3 keys │Home│ ↑  │PgUp│    │
     *         ├────┼────┼────┤    │
     *  4 keys │ ←  │    │ →  │    |
     *         ├────┼────┼────┼────┤
     *  3 keys │End │ ↓  │PdDn│    │
     *         ├────┴────┼────┤    │
     *  3 keys │ Insert  │Del │    │
     *         └─────────┴────┴────┴
     * 21 total
     */
    [_FN] = LAYOUT(
        _______, KC_MYCM, _______, _______,
        _______, _______, _______, _______,
        _______, _______, _______,
        _______, _______, _______, _______,
        _______, _______, _______,
            _______,      _______, _______
    )
};

bool led_update_user(led_t led_state) {
    // Change layer if numlock state changes, either triggered by OS or by numlock key on this keyboard
    if (led_state.num_lock) {
        layer_off(_FN);
    } else {
        layer_on(_FN);
    }
    return true;
}

#ifdef VIA_ENABLE
void via_custom_value_command_kb(uint8_t *data, uint8_t length) {
    // data = [ command_id, channel_id, value_id, value_data ]
    // data[0] = Command ID (0x07: id_custom_set_value, 0x08: id_custom_get_value, 0x09: id_custom_save)
    // data[1] = Channel ID (0x00: id_custom_channel is typical for custom user data)

    if ((data[0] == id_custom_set_value || data[0] == id_custom_get_value) && data[1] == id_custom_channel) {
        // Leave `data` unmodified to echo the payload back to host.
        // VIA calls raw_hid_send(data, length) immediately after this function returns.
        return;
    }

    // flag unhandled channels so VIA drops them
    data[0] = id_unhandled;
}
#endif
