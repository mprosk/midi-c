# midi-c
Simple byte-wise MIDI message decoder ideal for embedded systems.

##### **Current Limitations and Considerations**

- The parser does not enforce any behavior relating to Channel Mode messages. Your application must choose to implement behavior around Omni On/Off, Poly and Mono mode, etc.
- The parser does not decode System Exclusive messages, it will only flag the start and end of a SysEx chunk. If you need to receive SysEx messages, your application must collect the bytes and interpret them itself.

### Example Implementation

```c
#include "midi.h"

void main() {
  midi_parser_t parser = {0};
  midi_parser_init(&parser);
  midi_message_t message = {0};
  while (1) {
    // ...
    if (your_uart_handler_has_byte_available()) {
      uint8_t byte = your_uart_handler_get_byte();
     	midi_parser_read_byte(&parser, byte, &message);
      handle_midi_message(&message);
    }
    // ...
  }
}

void handle_midi_message(midi_message_t *message) {
  switch (message.message_type) {
    case MIDI_MESSAGE_NOTE_ON:
      your_note_on_logic(message.note, message.velocity);
      break;
    case MIDI_MESSAGE_NOTE_OFF:
      your_note_off_logic(message.note);
      break;
    case MIDI_CONTROL_CHANGE:
      switch (message.controller) {
        case MIDI_CC_SUSTAIN_PEDAL:
          your_hold_logic(message.control_value);
          break;
        default:
          break;
      }
    case MIDI_TIMING_CLOCK:
      your_clk_tick_handler();
      break;
    // You only have to respond to the messages you care about
    default:
      break;
  }
}
```



# Developing on this project

## Build

```bash
mkdir build
cd build
cmake ..
make
```

## Tests

Tesing is handled using the Unity framework.

Run tests by first running the build, then in the build dir, run

```bash
./test_midi
```
