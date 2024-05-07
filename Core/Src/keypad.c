#include "stm32l4xx_hal.h"
#include "keypad.h"

void keypad_init(void) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* Configure the keypad rows as output pins */
  GPIO_InitStruct.Pin = ROW_PINS;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* Configure the keypad columns as input pins */
  GPIO_InitStruct.Pin = COL_PINS;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void keypad_debounce(void) {
  /* Debounce key by waiting for 100ms */
  HAL_Delay(100);
}

PressedKey keypad_scan(void) {
  /* Set all columns high */
  HAL_GPIO_WritePin(COL_PORT, COL_PINS, GPIO_PIN_SET);
  // Scan each row for pressed key
  for (int row = 0; row < NUM_OF_ROWS; row++) {
    // Set current row low
    HAL_GPIO_WritePin(ROW_PORT, (1 << row), GPIO_PIN_RESET);

    // Check for pressed key in the current row
    for (int col = 0; col < NUM_OF_COLS; col++) {
      if (HAL_GPIO_ReadPin(COL_PORT, (1 << col)) == GPIO_PIN_RESET) {
        // Key is pressed, return the key value based on row and column
        HAL_GPIO_WritePin(ROW_PORT, ROW_PINS, GPIO_PIN_SET);  // Set all rows high again
        keypad_debounce(); // Debounce key
        return (row << 2) | col;  // Encode row and column into key value
      }
    }

    // Set current row high again
    HAL_GPIO_WritePin(ROW_PORT, (1 << row), GPIO_PIN_SET);
  }

  keypad_debounce(); // Debounce key
  return -1;
}
