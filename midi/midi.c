/***********************************************************************
 * @file midi.c
 * @brief MIDI parser implementation
 *
 * @details MIDI parser implementation for parsing MIDI 1.0 messages
 *          byte-by-byte using a state machine approach. Supports channel
 *          voice messages, system messages, and running status.
 *
 * @see MIDI 1.0 Detailed Specification
 *      https://midi.org/midi-1-0-detailed-specification
 ***********************************************************************/

/*=====================================================================*
    Local Header Files
 *=====================================================================*/
#include "midi.h"

/*====================================================================*
 *    Interface Header Files
 *====================================================================*/
/* None */

/*====================================================================*
 *    System-wide Header Files
 *====================================================================*/
#include <stddef.h>

/*=====================================================================*
    Private Defines
 *=====================================================================*/

/**
 * @brief MIDI Most Significant Nibble Mask
 * @details The mask for the most significant nibble of a MIDI status byte
 */
#define MIDI_MSN_MASK (0xF0)

/**
 * @brief MIDI Least Significant Nibble Mask
 * @details The mask for the least significant nibble of a MIDI status byte
 */
#define MIDI_LSN_MASK (0x0F)

/**
 * @brief MIDI Channel Mask
 * @details The mask for the channel component of a MIDI channel message
 */
#define MIDI_CHANNEL_MASK (0x0F)

/**
 * @brief MIDI Most Significant Bit Mask
 * @details The mask for the most significant bit of a MIDI byte
 */
#define MIDI_MSB_MASK (0x80)

/**
 * @brief MIDI Parser Buffer Size
 * @details The maximum buffer size for midi_parser_t.buffer
 */
#define MIDI_BUFFER_SIZE (2)

/**
 * @brief MIDI Maximum Data Byte Value
 * @details The maximum valid data byte value
 */
#define MIDI_MAX_DATA_BYTE (0x7F)

/*=====================================================================*
    Private Data Types
 *=====================================================================*/
/* None */

/*=====================================================================*
    Private Function Prototypes
 *=====================================================================*/

/* MIDI Parser Internal Functions */
static midi_message_type_t parse_status_byte(midi_parser_t *parser,
                                             const uint8_t byte,
                                             midi_message_t *message);

static midi_message_type_t parse_data_byte(midi_parser_t *parser,
                                           const uint8_t byte,
                                           midi_message_t *message);

/*=====================================================================*
    Private Data
 *=====================================================================*/
/* None */

/*=====================================================================*
    Public Function Implementations
 *=====================================================================*/

/**
 * @brief Initialize a MIDI parser
 * @param [in, out] parser Pointer to a midi_parser_t struct to initialize
 * @note This function must be called before using the parser
 */
void midi_parser_init(midi_parser_t *parser)
{
    if (parser == NULL) { return; }

    parser->message_type = MIDI_MESSAGE_NONE;
    parser->channel = MIDI_CHANNEL_NONE;
    parser->active_channel = MIDI_CHANNEL_NONE;
    parser->buffer[0] = 0;
    parser->buffer[1] = 0;
    parser->byte_count = 0;
}

/**
 * @brief Reset a MIDI parser to its initial state
 * @param [in,out] parser Pointer to a midi_parser_t struct to reset
 * @note This clears any partial message state and running status
 */
void midi_parser_reset(midi_parser_t *parser) { midi_parser_init(parser); }

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
                                    midi_message_t *message)
{
    /* Check for NULL pointers */
    if (parser == NULL || message == NULL) { return MIDI_MESSAGE_NONE; }

    /* Check if we got a status byte */
    if (byte & MIDI_MSB_MASK) {
        return parse_status_byte(parser, byte, message);
    } else {
        /* It's a data byte */
        return parse_data_byte(parser, byte, message);
    }
}

/*=====================================================================*
    Private Function Implementations
 *=====================================================================*/

/**
 * @brief Parse a MIDI status byte
 * @param [in,out] parser Pointer to a midi_parser_t struct
 * @param [in] byte The byte to parse
 * @note This function assumes that the byte is a status byte
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
static midi_message_type_t parse_status_byte(midi_parser_t *parser,
                                             const uint8_t byte,
                                             midi_message_t *message)
{
    /* Check for NULL pointers */
    if (parser == NULL || message == NULL) { return MIDI_MESSAGE_NONE; }

    switch (byte & MIDI_MSN_MASK) {

    /* CHANNEL VOICE MESSAGES */
    case MIDI_MESSAGE_NOTE_OFF:
    case MIDI_MESSAGE_NOTE_ON:
    case MIDI_MESSAGE_KEY_PRESSURE:
    case MIDI_MESSAGE_CONTROL_CHANGE:
    case MIDI_MESSAGE_PROGRAM_CHANGE:
    case MIDI_MESSAGE_CHANNEL_PRESSURE:
    case MIDI_MESSAGE_PITCH_BEND:
        parser->message_type = (midi_message_type_t)(byte & MIDI_MSN_MASK);
        parser->channel = (midi_channel_t)(byte & MIDI_CHANNEL_MASK);
        parser->byte_count = 0;
        break;

    /* SYSTEM MESSAGES */
    case MIDI_MESSAGE_SYSTEM_EXCLUSIVE:
        /*
         * Because the previous switch was using the masked byte,
         * all system messages are handled here,
         * matching on the full byte value
         */
        switch (byte) {

        case MIDI_MESSAGE_SYSTEM_EXCLUSIVE:
            parser->message_type = MIDI_MESSAGE_SYSTEM_EXCLUSIVE;
            parser->byte_count = 0;

            message->message_type = MIDI_MESSAGE_SYSTEM_EXCLUSIVE;
            message->channel = MIDI_CHANNEL_NONE;
            return MIDI_MESSAGE_SYSTEM_EXCLUSIVE;

        /* SYSTEM COMMON MESSAGES */
        case MIDI_MESSAGE_MTC_QUARTER_FRAME:
        case MIDI_MESSAGE_SONG_POSITION_POINTER:
        case MIDI_MESSAGE_SONG_SELECT:
            parser->message_type = (midi_message_type_t)byte;
            parser->channel = MIDI_CHANNEL_NONE;
            parser->byte_count = 0;
            break;
        case MIDI_MESSAGE_TUNE_REQUEST:
        case MIDI_MESSAGE_END_OF_EXCLUSIVE:
            parser->message_type = MIDI_MESSAGE_NONE;
            parser->channel = MIDI_CHANNEL_NONE;
            parser->byte_count = 0;
            message->message_type = (midi_message_type_t)byte;
            message->channel = MIDI_CHANNEL_NONE;
            return message->message_type;

        /* SYSTEM REALTIME MESSAGES */
        case MIDI_MESSAGE_TIMING_CLOCK:
        case MIDI_MESSAGE_START:
        case MIDI_MESSAGE_CONTINUE:
        case MIDI_MESSAGE_STOP:
        case MIDI_MESSAGE_ACTIVE_SENSE:
        case MIDI_MESSAGE_SYSTEM_RESET:
            /* Do not clear running status */
            message->message_type = (midi_message_type_t)byte;
            message->channel = MIDI_CHANNEL_NONE;
            return message->message_type;

        default:
            /* Undefined system message type */
            /* Do not reset running status */
            break;
        }
        break;

    default:
        /* Undefined message type */
        /* Do not reset running status */
        break;
    }
    return MIDI_MESSAGE_NONE;
}

/**
 * @brief Parse a MIDI data byte
 * @param [in,out] parser Pointer to a midi_parser_t struct
 * @param [in] byte The byte to parse
 * @note This function assumes that the byte is a data byte
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
static midi_message_type_t parse_data_byte(midi_parser_t *parser,
                                           const uint8_t byte,
                                           midi_message_t *message)
{
    /* Check for NULL pointers */
    if (parser == NULL || message == NULL) { return MIDI_MESSAGE_NONE; }

    /* Validate data byte range */
    if (byte > MIDI_MAX_DATA_BYTE) {
        /* Invalid data byte - ignore */
        /* this should literally never happen */
        return MIDI_MESSAGE_NONE;
    }

    /* Buffer overflow protection */
    if (parser->byte_count >= MIDI_BUFFER_SIZE) {
        /* this should never happen */
        parser->byte_count = 0;
        return MIDI_MESSAGE_NONE;
    }

    /*
     * Figure out what to do with the data byte
     * based on the current message type
     */
    switch (parser->message_type) {

    case MIDI_MESSAGE_NOTE_OFF:
        parser->buffer[parser->byte_count++] = byte;
        if (parser->byte_count != 2) { break; }
        message->message_type = MIDI_MESSAGE_NOTE_OFF;
        message->channel = parser->channel;
        message->note = parser->buffer[0];
        message->velocity = parser->buffer[1];
        parser->byte_count = 0;
        return MIDI_MESSAGE_NOTE_OFF;

    case MIDI_MESSAGE_NOTE_ON:
        parser->buffer[parser->byte_count++] = byte;
        if (parser->byte_count != 2) { break; }
        message->channel = parser->channel;
        message->note = parser->buffer[0];
        message->velocity = parser->buffer[1];
        if (message->velocity == 0) {
            /* Note on with velocity 0 is equivalent to note off */
            message->message_type = MIDI_MESSAGE_NOTE_OFF;
        } else {
            message->message_type = MIDI_MESSAGE_NOTE_ON;
        }
        parser->byte_count = 0;
        return message->message_type;

    case MIDI_MESSAGE_KEY_PRESSURE:
        parser->buffer[parser->byte_count++] = byte;
        if (parser->byte_count != 2) { break; }
        message->message_type = MIDI_MESSAGE_KEY_PRESSURE;
        message->channel = parser->channel;
        message->key = parser->buffer[0];
        message->key_pressure = parser->buffer[1];
        parser->byte_count = 0;
        return MIDI_MESSAGE_KEY_PRESSURE;

    case MIDI_MESSAGE_CONTROL_CHANGE:
        parser->buffer[parser->byte_count++] = byte;
        if (parser->byte_count != 2) { break; }
        message->message_type = MIDI_MESSAGE_CONTROL_CHANGE;
        message->channel = parser->channel;
        message->controller = (midi_controller_t)parser->buffer[0];
        message->control_value = parser->buffer[1];
        parser->byte_count = 0;
        switch (message->controller) {
        case MIDI_CC_ALL_SOUND_OFF:
        case MIDI_CC_RESET_ALL_CONTROLLERS:
        case MIDI_CC_LOCAL_CONTROL:
        case MIDI_CC_ALL_NOTES_OFF:
        case MIDI_CC_OMNI_OFF:
        case MIDI_CC_OMNI_ON:
        case MIDI_CC_MONO_ON:
        case MIDI_CC_POLY_ON:
            message->message_type = (midi_message_type_t)message->controller;
            break;
        default:
            break;
        }
        return message->message_type;

    case MIDI_MESSAGE_PROGRAM_CHANGE:
        message->message_type = MIDI_MESSAGE_PROGRAM_CHANGE;
        message->channel = parser->channel;
        message->program = byte;
        parser->byte_count = 0;
        return MIDI_MESSAGE_PROGRAM_CHANGE;

    case MIDI_MESSAGE_CHANNEL_PRESSURE:
        message->message_type = MIDI_MESSAGE_CHANNEL_PRESSURE;
        message->channel = parser->channel;
        message->channel_pressure = byte;
        parser->byte_count = 0;
        return MIDI_MESSAGE_CHANNEL_PRESSURE;

    case MIDI_MESSAGE_PITCH_BEND:
        parser->buffer[parser->byte_count++] = byte;
        if (parser->byte_count != 2) { break; }
        message->message_type = MIDI_MESSAGE_PITCH_BEND;
        message->channel = parser->channel;
        message->pitch_bend = parser->buffer[1] << 7 | parser->buffer[0];
        parser->byte_count = 0;
        return MIDI_MESSAGE_PITCH_BEND;

    case MIDI_MESSAGE_SYSTEM_EXCLUSIVE:
        /* Just ignore the data byte */
        return MIDI_MESSAGE_NONE;

    case MIDI_MESSAGE_MTC_QUARTER_FRAME:
        parser->message_type = MIDI_MESSAGE_NONE;
        parser->byte_count = 0;
        message->message_type = MIDI_MESSAGE_MTC_QUARTER_FRAME;
        message->channel = MIDI_CHANNEL_NONE;
        message->mtc_msg_type = byte >> 4;
        message->mtc_values = byte & MIDI_LSN_MASK;
        return MIDI_MESSAGE_MTC_QUARTER_FRAME;

    case MIDI_MESSAGE_SONG_POSITION_POINTER:
        parser->buffer[parser->byte_count++] = byte;
        if (parser->byte_count != 2) { break; }
        parser->message_type = MIDI_MESSAGE_NONE;
        parser->byte_count = 0;
        message->message_type = MIDI_MESSAGE_SONG_POSITION_POINTER;
        message->channel = MIDI_CHANNEL_NONE;
        message->song_position = parser->buffer[1] << 7 | parser->buffer[0];
        return message->message_type;

    case MIDI_MESSAGE_SONG_SELECT:
        parser->message_type = MIDI_MESSAGE_NONE;
        parser->byte_count = 0;
        message->message_type = MIDI_MESSAGE_SONG_SELECT;
        message->channel = MIDI_CHANNEL_NONE;
        message->song_select = byte;
        return MIDI_MESSAGE_SONG_SELECT;

    default:
        /* Unexpected data byte at this time */
        break;
    }

    return MIDI_MESSAGE_NONE;
}
