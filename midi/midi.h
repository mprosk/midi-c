/**********************************************************************
 * @file midi.h
 * @brief MIDI module
 *
 * @details This module provides a simple interface for sending and
 *          receiving MIDI messages.
 *
 * @see MIDI 1.0 Detailed Specification
 *      https://midi.org/midi-1-0-detailed-specification
 **********************************************************************/

#ifndef MIDI_H
#define MIDI_H

/*=====================================================================*
    Required Header Files
 *=====================================================================*/
#include <stdint.h>

/*=====================================================================*
    Public Defines
 *=====================================================================*/
/* None */

/*=====================================================================*
    Public Data Types
 *=====================================================================*/

/**
 * @brief MIDI Channel
 *
 * @details This enum defines the MIDI channels
 *          The channels are numbered from 1 to 16
 */
typedef enum midi_channel_t {
    MIDI_CHANNEL_1 = 0x00,
    MIDI_CHANNEL_2 = 0x01,
    MIDI_CHANNEL_3 = 0x02,
    MIDI_CHANNEL_4 = 0x03,
    MIDI_CHANNEL_5 = 0x04,
    MIDI_CHANNEL_6 = 0x05,
    MIDI_CHANNEL_7 = 0x06,
    MIDI_CHANNEL_8 = 0x07,
    MIDI_CHANNEL_9 = 0x08,
    MIDI_CHANNEL_10 = 0x09,
    MIDI_CHANNEL_11 = 0x0A,
    MIDI_CHANNEL_12 = 0x0B,
    MIDI_CHANNEL_13 = 0x0C,
    MIDI_CHANNEL_14 = 0x0D,
    MIDI_CHANNEL_15 = 0x0E,
    MIDI_CHANNEL_16 = 0x0F,
    /**
     * @brief MIDI Channel None
     * @details This placeholder value is used to indicate
     *          that no channel is selected or when a channel
     *          number is not applicable for the message type
     */
    MIDI_CHANNEL_NONE = 0xFF,

} midi_channel_t;

/**
 * @brief MIDI Message Type
 *
 * @details This enum defines the MIDI message types
 */
typedef enum midi_message_type_t {

    /**
     * @brief MIDI Message None
     * @details This placeholder value is used to indicate
     *          that no message is detected
     */
    MIDI_MESSAGE_NONE = 0x00,

    /**
     * @brief Note Off
     * @param note The MIDI note number to turn off
     * @param velocity The release velocity of the note if applicable
     */
    MIDI_MESSAGE_NOTE_OFF = 0x80,

    /**
     * @brief Note On
     * @param note The MIDI note number to turn on
     * @param velocity The note velocity
     */
    MIDI_MESSAGE_NOTE_ON = 0x90,

    /**
     * @brief Polyphonic Key Pressure
     * @param key The MIDI note number to set the pressure of
     * @param key_pressure The pressure of the note
     */
    MIDI_MESSAGE_KEY_PRESSURE = 0xA0,

    /**
     * @brief Control Change
     * @param controller The controller number to change
     * @param control_value The value to set on the controller
     * @note Channel Mode messages are treated as their own
     *       top-level entry in `midi_message_type_t`
     */
    MIDI_MESSAGE_CONTROL_CHANGE = 0xB0,

    /**
     * @brief All Sound Off
     */
    MIDI_MESSAGE_ALL_SOUND_OFF = 0x78, /* 120 */

    /**
     * @brief Reset All Controllers
     */
    MIDI_MESSAGE_RESET_ALL_CONTROLLERS = 0x79, /* 121 */

    /**
     * @brief Local Control On/Off
     */
    MIDI_MESSAGE_LOCAL_CONTROL = 0x7A, /* 122 */

    /**
     * @brief All Notes Off
     * @note Also functions as All Notes Off (MIDI 1.0 Spec, Pg 20)
     */
    MIDI_MESSAGE_ALL_NOTES_OFF = 0x7B, /* 123 */

    /**
     * @brief Omni Mode Off
     * @note Also functions as All Notes Off (MIDI 1.0 Spec, Pg 20)
     */
    MIDI_MESSAGE_OMNI_OFF = 0x7C, /* 124 */

    /**
     * @brief Omni Mode On
     * @note Also functions as All Notes Off (MIDI 1.0 Spec, Pg 20)
     */
    MIDI_MESSAGE_OMNI_ON = 0x7D, /* 125 */

    /**
     * @brief Mono Mode On
     * @note Also functions as All Notes Off (MIDI 1.0 Spec, Pg 20)
     */
    MIDI_MESSAGE_MONO_ON = 0x7E, /* 126 */

    /**
     * @brief Poly Mode On
     * @note Also functions as All Notes Off (MIDI 1.0 Spec, Pg 20)
     */
    MIDI_MESSAGE_POLY_ON = 0x7F, /* 127 */

    /**
     * @brief Program Change
     * @param program The program number to change to
     */
    MIDI_MESSAGE_PROGRAM_CHANGE = 0xC0,

    /**
     * @brief Channel Pressure
     * @param channel_pressure The pressure to set on the channel
     */
    MIDI_MESSAGE_CHANNEL_PRESSURE = 0xD0,

    /**
     * @brief Pitch Bend
     * @param pitch_bend The pitch bend value
     */
    MIDI_MESSAGE_PITCH_BEND = 0xE0,

    /**
     * @brief Start of System Exclusive
     */
    MIDI_MESSAGE_SYSTEM_EXCLUSIVE = 0xF0,

    /**
     * @brief MTC Quarter Frame
     */
    MIDI_MESSAGE_MTC_QUARTER_FRAME = 0xF1,

    /**
     * @brief Song Position Pointer
     * @param song_position The position in the song
     */
    MIDI_MESSAGE_SONG_POSITION_POINTER = 0xF2,

    /**
     * @brief Song Select
     * @param song_select The song number to select
     */
    MIDI_MESSAGE_SONG_SELECT = 0xF3,

    /**
     * @brief Tune Request
     */
    MIDI_MESSAGE_TUNE_REQUEST = 0xF6,

    /**
     * @brief End of System Exclusive
     */
    MIDI_MESSAGE_END_OF_EXCLUSIVE = 0xF7,

    /**
     * @brief Timing Clock
     */
    MIDI_MESSAGE_TIMING_CLOCK = 0xF8,

    /**
     * @brief Start
     */
    MIDI_MESSAGE_START = 0xFA,

    /**
     * @brief Continue
     */
    MIDI_MESSAGE_CONTINUE = 0xFB,

    /**
     * @brief Stop
     */
    MIDI_MESSAGE_STOP = 0xFC,

    /**
     * @brief Active Sense
     */
    MIDI_MESSAGE_ACTIVE_SENSE = 0xFE,

    /**
     * @brief System Reset
     */
    MIDI_MESSAGE_SYSTEM_RESET = 0xFF,

} midi_message_type_t;

/**
 * @brief MIDI Controller Names
 * @details This enum defines the MIDI controllers names
 */
typedef enum midi_controller_t {
    MIDI_CC_BANK_SELECT = 0x00,
    MIDI_CC_MOD_WHEEL = 0x01,
    MIDI_CC_BREATH_CONTROLLER = 0x02,
    MIDI_CC_UNDEFINED_03 = 0x03,
    MIDI_CC_FOOT_CONTROLLER = 0x04,
    MIDI_CC_PORTAMENTO_TIME = 0x05,
    MIDI_CC_DATA_ENTRY_MSB = 0x06,
    MIDI_CC_CHANNEL_VOLUME = 0x07,
    MIDI_CC_BALANCE = 0x08,
    MIDI_CC_UNDEFINED_09 = 0x09,
    MIDI_CC_PAN = 0x0A,
    MIDI_CC_EXPRESSION_CONTROLLER = 0x0B,
    MIDI_CC_EFFECT_CONTROL_1 = 0x0C,
    MIDI_CC_EFFECT_CONTROL_2 = 0x0D,
    MIDI_CC_UNDEFINED_0E = 0x0E,
    MIDI_CC_UNDEFINED_0F = 0x0F,
    MIDI_CC_GENERAL_PURPOSE_1 = 0x10,
    MIDI_CC_GENERAL_PURPOSE_2 = 0x11,
    MIDI_CC_GENERAL_PURPOSE_3 = 0x12,
    MIDI_CC_GENERAL_PURPOSE_4 = 0x13,
    MIDI_CC_UNDEFINED_14 = 0x14,
    MIDI_CC_UNDEFINED_15 = 0x15,
    MIDI_CC_UNDEFINED_16 = 0x16,
    MIDI_CC_UNDEFINED_17 = 0x17,
    MIDI_CC_UNDEFINED_18 = 0x18,
    MIDI_CC_UNDEFINED_19 = 0x19,
    MIDI_CC_UNDEFINED_1A = 0x1A,
    MIDI_CC_UNDEFINED_1B = 0x1B,
    MIDI_CC_UNDEFINED_1C = 0x1C,
    MIDI_CC_UNDEFINED_1D = 0x1D,
    MIDI_CC_UNDEFINED_1E = 0x1E,
    MIDI_CC_UNDEFINED_1F = 0x1F,
    MIDI_CC_BANK_SELECT_LSB = 0x20,
    MIDI_CC_MOD_WHEEL_LSB = 0x21,
    MIDI_CC_BREATH_CONTROLLER_LSB = 0x22,
    MIDI_CC_UNDEFINED_23 = 0x23,
    MIDI_CC_FOOT_CONTROLLER_LSB = 0x24,
    MIDI_CC_PORTAMENTO_TIME_LSB = 0x25,
    MIDI_CC_DATA_ENTRY_LSB = 0x26,
    MIDI_CC_CHANNEL_VOLUME_LSB = 0x27,
    MIDI_CC_BALANCE_LSB = 0x28,
    MIDI_CC_UNDEFINED_29 = 0x29,
    MIDI_CC_PAN_LSB = 0x2A,
    MIDI_CC_EXPRESSION_CONTROLLER_LSB = 0x2B,
    MIDI_CC_EFFECT_CONTROL_1_LSB = 0x2C,
    MIDI_CC_EFFECT_CONTROL_2_LSB = 0x2D,
    MIDI_CC_UNDEFINED_2E = 0x2E,
    MIDI_CC_UNDEFINED_2F = 0x2F,
    MIDI_CC_GENERAL_PURPOSE_1_LSB = 0x30,
    MIDI_CC_GENERAL_PURPOSE_2_LSB = 0x31,
    MIDI_CC_GENERAL_PURPOSE_3_LSB = 0x32,
    MIDI_CC_GENERAL_PURPOSE_4_LSB = 0x33,
    MIDI_CC_UNDEFINED_34 = 0x34,
    MIDI_CC_UNDEFINED_35 = 0x35,
    MIDI_CC_UNDEFINED_36 = 0x36,
    MIDI_CC_UNDEFINED_37 = 0x37,
    MIDI_CC_UNDEFINED_38 = 0x38,
    MIDI_CC_UNDEFINED_39 = 0x39,
    MIDI_CC_UNDEFINED_3A = 0x3A,
    MIDI_CC_UNDEFINED_3B = 0x3B,
    MIDI_CC_UNDEFINED_3C = 0x3C,
    MIDI_CC_UNDEFINED_3D = 0x3D,
    MIDI_CC_UNDEFINED_3E = 0x3E,
    MIDI_CC_UNDEFINED_3F = 0x3F,
    MIDI_CC_SUSTAIN_PEDAL = 0x40,
    MIDI_CC_PORTAMENTO_ON_OFF = 0x41,
    MIDI_CC_SOSTENUTO = 0x42,
    MIDI_CC_SOFT_PEDAL = 0x43,
    MIDI_CC_LEGATO_FOOTSWITCH = 0x44,
    MIDI_CC_HOLD_2 = 0x45,
    MIDI_CC_SOUND_CTRL_1_VARIATION = 0x46,
    MIDI_CC_SOUND_CTRL_2_TIMBRE = 0x47,
    MIDI_CC_SOUND_CTRL_3_RELEASE_TIME = 0x48,
    MIDI_CC_SOUND_CTRL_4_ATTACK_TIME = 0x49,
    MIDI_CC_SOUND_CTRL_5_BRIGHTNESS = 0x4A,
    MIDI_CC_SOUND_CTRL_6 = 0x4B,
    MIDI_CC_SOUND_CTRL_7 = 0x4C,
    MIDI_CC_SOUND_CTRL_8 = 0x4D,
    MIDI_CC_SOUND_CTRL_9 = 0x4E,
    MIDI_CC_SOUND_CTRL_10 = 0x4F,
    MIDI_CC_GENERAL_PURPOSE_5 = 0x50,
    MIDI_CC_GENERAL_PURPOSE_6 = 0x51,
    MIDI_CC_GENERAL_PURPOSE_7 = 0x52,
    MIDI_CC_GENERAL_PURPOSE_8 = 0x53,
    MIDI_CC_PORTAMENTO_CONTROL = 0x54,
    MIDI_CC_UNDEFINED_55 = 0x55,
    MIDI_CC_UNDEFINED_56 = 0x56,
    MIDI_CC_UNDEFINED_57 = 0x57,
    MIDI_CC_UNDEFINED_58 = 0x58,
    MIDI_CC_UNDEFINED_59 = 0x59,
    MIDI_CC_UNDEFINED_5A = 0x5A,
    MIDI_CC_EFFECT_1_DEPTH = 0x5B,
    MIDI_CC_EFFECT_2_DEPTH = 0x5C,
    MIDI_CC_EFFECT_3_DEPTH = 0x5D,
    MIDI_CC_EFFECT_4_DEPTH = 0x5E,
    MIDI_CC_EFFECT_5_DEPTH = 0x5F,
    MIDI_CC_DATA_INCREMENT = 0x60,
    MIDI_CC_DATA_DECREMENT = 0x61,
    MIDI_CC_NRPN_LSB = 0x62,
    MIDI_CC_NRPN_MSB = 0x63,
    MIDI_CC_RPN_LSB = 0x64,
    MIDI_CC_RPN_MSB = 0x65,
    MIDI_CC_UNDEFINED_66 = 0x66,
    MIDI_CC_UNDEFINED_67 = 0x67,
    MIDI_CC_UNDEFINED_68 = 0x68,
    MIDI_CC_UNDEFINED_69 = 0x69,
    MIDI_CC_UNDEFINED_6A = 0x6A,
    MIDI_CC_UNDEFINED_6B = 0x6B,
    MIDI_CC_UNDEFINED_6C = 0x6C,
    MIDI_CC_UNDEFINED_6D = 0x6D,
    MIDI_CC_UNDEFINED_6E = 0x6E,
    MIDI_CC_UNDEFINED_6F = 0x6F,
    MIDI_CC_UNDEFINED_70 = 0x70,
    MIDI_CC_UNDEFINED_71 = 0x71,
    MIDI_CC_UNDEFINED_72 = 0x72,
    MIDI_CC_UNDEFINED_73 = 0x73,
    MIDI_CC_UNDEFINED_74 = 0x74,
    MIDI_CC_UNDEFINED_75 = 0x75,
    MIDI_CC_UNDEFINED_76 = 0x76,
    MIDI_CC_UNDEFINED_77 = 0x77,
    MIDI_CC_ALL_SOUND_OFF = 0x78,
    MIDI_CC_RESET_ALL_CONTROLLERS = 0x79,
    MIDI_CC_LOCAL_CONTROL = 0x7A,
    MIDI_CC_ALL_NOTES_OFF = 0x7B,
    MIDI_CC_OMNI_OFF = 0x7C,
    MIDI_CC_OMNI_ON = 0x7D,
    MIDI_CC_MONO_ON = 0x7E,
    MIDI_CC_POLY_ON = 0x7F,
} midi_controller_t;

/**
 * @brief MIDI Message
 * @details This struct defines a MIDI message
 *          and contains its associated data
 */
typedef struct midi_message_t {
    /**
     * @brief MIDI Message Type
     */
    midi_message_type_t message_type;

    /**
     * @brief MIDI Channel Number
     * @details The MIDI channel number associated with the message
     *          If channels are not relevant to the specific message
     *          type then this will be set to MIDI_CHANNEL_NONE
     */
    midi_channel_t channel;

    /* Anonymous union to store the data associated with the message */
    union {
        /**
         * @brief Note Off, Note On
         */
        struct {
            /**
             * @brief MIDI Note Number
             */
            uint8_t note;
            /**
             * @brief Note velocity
             */
            uint8_t velocity;
        };

        /**
         * @brief Polyphonic Key Pressure
         */
        struct {
            /**
             * @brief MIDI Note Number
             */
            uint8_t key;
            /**
             * @brief Note pressure
             */
            uint8_t key_pressure;
        };

        /**
         * @brief Control Change
         */
        struct {
            /**
             * @brief Controller
             * @details The number of the controller to change
             */
            midi_controller_t controller;
            /**
             * @brief Control Value
             * @details The value to set on the controller
             */
            uint8_t control_value;
        };

        /**
         * @brief Program Change
         * @details The program number to change to
         */
        uint8_t program;

        /**
         * @brief Channel Pressure
         * @details The pressure to set on the channel
         */
        uint8_t channel_pressure;

        /**
         * @brief Pitch Bend
         * @details The pitch bend value
         */
        uint16_t pitch_bend;

        /**
         * @brief Song Position Pointer
         * @details The position in the song
         */
        uint16_t song_position;

        /**
         * @brief Song Select
         * @details The song number to select
         */
        uint8_t song_select;

        /**
         * @brief MTC Quarter Frame
         */
        struct {
            uint8_t mtc_msg_type;
            uint8_t mtc_values;
        };
    };
} midi_message_t;

/**
 * @brief MIDI Parser
 * @details This struct contains the internal state of the MIDI parser
 * @note The fields of this struct should not be accessed directly.
 *       Use the `midi_parse_byte` function to parse MIDI bytes
 */
typedef struct midi_parser_t {
    midi_message_type_t message_type;
    midi_channel_t channel;
    midi_channel_t active_channel;
    uint8_t buffer[2];
    uint8_t byte_count;
} midi_parser_t;

/*=====================================================================*
    Public Functions
 *=====================================================================*/

/**
 * @brief Initialize a MIDI parser
 * @param [in, out] parser Pointer to a midi_parser_t struct to initialize
 * @note This function must be called before using the parser
 */
void midi_parser_init(midi_parser_t *parser);

/**
 * @brief Reset a MIDI parser to its initial state
 * @param [in,out] parser Pointer to a midi_parser_t struct to reset
 * @note This clears any partial message state and running status
 */
void midi_parser_reset(midi_parser_t *parser);

/**
 * @brief Set the active channel for the MIDI parser
 * @details Causes the parser to only return channel messages
 * @param [in,out] parser Pointer to a midi_parser_t struct
 * @param [in] channel The MIDI channel to set as active
 * @note Setting this value to MIDI_CHANNEL_NONE
 *       will disable the active channel
 *       and the parser will respond to all channels (omni mode)
 */
void midi_parser_set_active_channel(midi_parser_t *parser,
                                    midi_channel_t channel);

/**
 * @brief Get the active channel for the MIDI parser
 * @param [in] parser Pointer to a midi_parser_t struct
 * @return The active channel
 */
midi_channel_t midi_parser_get_active_channel(midi_parser_t *parser);

/**
 * @brief Parse a MIDI byte
 * @param [in,out] parser Pointer to a midi_parser_t struct
 * @param [in] byte The byte to parse
 * @param [out] message Pointer to a midi_message_t struct.
 *      This struct will be updated with the parsed message data,
 *      if a complete message has been parsed.
 * @return The MIDI message type. If a complete message has been
 *      parsed then the message type will be returned (and will be
 *      identical to the message_type field in the message struct).
 *      Otherwise the message type will be MIDI_MESSAGE_NONE
 * @retval MIDI_MESSAGE_NONE: No complete message has been parsed
 * @retval others: The message type of the parsed message
 */
midi_message_type_t midi_parse_byte(midi_parser_t *parser,
                                    const uint8_t byte,
                                    midi_message_t *message);

#endif /* MIDI_H */
