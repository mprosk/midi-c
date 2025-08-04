/***********************************************************************
 * @file test_midi.c
 * @brief Unit tests for the MIDI parser module
 ***********************************************************************/

/*=====================================================================*
    System-wide Header Files
 *=====================================================================*/
#include "unity.h"
#include <string.h>

/*=====================================================================*
    Local Header Files
 *=====================================================================*/
#include "../midi/midi.h"

/*=====================================================================*
    Private Data
 *=====================================================================*/
static midi_parser_t parser;
static midi_message_t message;

/*=====================================================================*
    Test Setup and Teardown
 *=====================================================================*/
void setUp(void)
{
    midi_parser_init(&parser);
    memset(&message, 0, sizeof(message));
}

void tearDown(void) { /* Nothing to tear down */ }

/*=====================================================================*
    Basic Tests
 *=====================================================================*/

/**
 * @brief MIDI parser initialization
 */
void test_midi_parser_init(void)
{
    midi_parser_t test_parser;

    midi_parser_init(&test_parser);

    TEST_ASSERT_EQUAL(MIDI_MESSAGE_NONE, test_parser.message_type);
    TEST_ASSERT_EQUAL(MIDI_CHANNEL_NONE, test_parser.channel);
    TEST_ASSERT_EQUAL(0, test_parser.byte_count);
}

/**
 * @brief MIDI parser null pointer handling
 */
void test_null_pointer_handling(void)
{
    midi_message_type_t result;

    /* Test NULL parser */
    result = midi_parse_byte(NULL, MIDI_MESSAGE_NOTE_ON, &message);
    TEST_ASSERT_EQUAL(MIDI_MESSAGE_NONE, result);

    /* Test NULL message */
    result = midi_parse_byte(&parser, MIDI_MESSAGE_NOTE_ON, NULL);
    TEST_ASSERT_EQUAL(MIDI_MESSAGE_NONE, result);

    /* Test NULL parser init */
    midi_parser_init(NULL);
}

/*=====================================================================*
    Channel Messages
 *=====================================================================*/

/**
 * @brief Test the note on message
 * @details Tests every possible note on message
 *          Checks that note on events with velocity zero are
 *          correctly interpreted as note off events
 *          Checks that running status works correctly
 */
void test_note_on_message(void)
{
    midi_message_type_t result;

    for (uint8_t channel = 0; channel < 16; channel++) {
        result = midi_parse_byte(
            &parser, MIDI_MESSAGE_NOTE_ON | channel, &message);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_NONE, result);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_NOTE_ON, parser.message_type);
        TEST_ASSERT_EQUAL(channel, parser.channel);
        TEST_ASSERT_EQUAL(0, parser.byte_count);

        for (uint8_t note = 0; note < 128; note++) {
            for (uint8_t velocity = 0; velocity < 128; velocity++) {
                result = midi_parse_byte(&parser, note, &message);
                TEST_ASSERT_EQUAL(MIDI_MESSAGE_NONE, result);
                TEST_ASSERT_EQUAL(MIDI_MESSAGE_NOTE_ON, parser.message_type);
                TEST_ASSERT_EQUAL(channel, parser.channel);
                TEST_ASSERT_EQUAL(1, parser.byte_count);

                result = midi_parse_byte(&parser, velocity, &message);
                if (velocity == 0) {
                    TEST_ASSERT_EQUAL(MIDI_MESSAGE_NOTE_OFF, result);
                    TEST_ASSERT_EQUAL(MIDI_MESSAGE_NOTE_OFF,
                                      message.message_type);
                } else {
                    TEST_ASSERT_EQUAL(MIDI_MESSAGE_NOTE_ON, result);
                    TEST_ASSERT_EQUAL(MIDI_MESSAGE_NOTE_ON,
                                      message.message_type);
                }
                TEST_ASSERT_EQUAL(MIDI_MESSAGE_NOTE_ON, parser.message_type);
                TEST_ASSERT_EQUAL(channel, parser.channel);
                TEST_ASSERT_EQUAL(0, parser.byte_count);

                TEST_ASSERT_EQUAL(channel, message.channel);
                TEST_ASSERT_EQUAL(note, message.note);
                TEST_ASSERT_EQUAL(velocity, message.velocity);
            }
        }
    }
}

/**
 * @brief Test the note off message
 * @details Tests every possible note off message
 *          Checks that running status works correctly
 */
void test_note_off_message(void)
{
    midi_message_type_t result;

    for (uint8_t channel = 0; channel < 16; channel++) {
        result = midi_parse_byte(
            &parser, MIDI_MESSAGE_NOTE_OFF | channel, &message);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_NONE, result);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_NOTE_OFF, parser.message_type);
        TEST_ASSERT_EQUAL(channel, parser.channel);
        TEST_ASSERT_EQUAL(0, parser.byte_count);

        for (uint8_t note = 0; note < 128; note++) {
            for (uint8_t velocity = 0; velocity < 128; velocity++) {
                result = midi_parse_byte(&parser, note, &message);
                TEST_ASSERT_EQUAL(MIDI_MESSAGE_NONE, result);
                TEST_ASSERT_EQUAL(MIDI_MESSAGE_NOTE_OFF, parser.message_type);
                TEST_ASSERT_EQUAL(channel, parser.channel);
                TEST_ASSERT_EQUAL(1, parser.byte_count);

                result = midi_parse_byte(&parser, velocity, &message);
                TEST_ASSERT_EQUAL(MIDI_MESSAGE_NOTE_OFF, result);
                TEST_ASSERT_EQUAL(MIDI_MESSAGE_NOTE_OFF, parser.message_type);
                TEST_ASSERT_EQUAL(channel, parser.channel);
                TEST_ASSERT_EQUAL(0, parser.byte_count);

                TEST_ASSERT_EQUAL(MIDI_MESSAGE_NOTE_OFF, message.message_type);
                TEST_ASSERT_EQUAL(channel, message.channel);
                TEST_ASSERT_EQUAL(note, message.note);
                TEST_ASSERT_EQUAL(velocity, message.velocity);
            }
        }
    }
}

/**
 * @brief Test the polyphonic key pressure message
 * @details Tests every possible polyphonic key pressure message
 *          Checks that running status works correctly
 */
void test_poly_key_pressure(void)
{
    midi_message_type_t result;

    for (uint8_t channel = 0; channel < 16; channel++) {
        result = midi_parse_byte(
            &parser, MIDI_MESSAGE_KEY_PRESSURE | channel, &message);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_NONE, result);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_KEY_PRESSURE, parser.message_type);
        TEST_ASSERT_EQUAL(channel, parser.channel);
        TEST_ASSERT_EQUAL(0, parser.byte_count);

        for (uint8_t key = 0; key < 128; key++) {
            for (uint8_t pressure = 0; pressure < 128; pressure++) {
                result = midi_parse_byte(&parser, key, &message);
                TEST_ASSERT_EQUAL(MIDI_MESSAGE_NONE, result);
                TEST_ASSERT_EQUAL(MIDI_MESSAGE_KEY_PRESSURE,
                                  parser.message_type);
                TEST_ASSERT_EQUAL(channel, parser.channel);
                TEST_ASSERT_EQUAL(1, parser.byte_count);

                result = midi_parse_byte(&parser, pressure, &message);
                TEST_ASSERT_EQUAL(MIDI_MESSAGE_KEY_PRESSURE, result);
                TEST_ASSERT_EQUAL(MIDI_MESSAGE_KEY_PRESSURE,
                                  parser.message_type);
                TEST_ASSERT_EQUAL(channel, parser.channel);
                TEST_ASSERT_EQUAL(0, parser.byte_count);

                TEST_ASSERT_EQUAL(MIDI_MESSAGE_KEY_PRESSURE,
                                  message.message_type);
                TEST_ASSERT_EQUAL(channel, message.channel);
                TEST_ASSERT_EQUAL(key, message.key);
                TEST_ASSERT_EQUAL(pressure, message.key_pressure);
            }
        }
    }
}

/**
 * @brief Test the control change message
 * @details Tests every possible control change message
 *          Checks that running status works correctly
 */
void test_control_change_message(void)
{
    midi_message_type_t result;

    for (uint8_t channel = 0; channel < 16; channel++) {
        result = midi_parse_byte(
            &parser, MIDI_MESSAGE_CONTROL_CHANGE | channel, &message);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_NONE, result);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_CONTROL_CHANGE, parser.message_type);
        TEST_ASSERT_EQUAL(channel, parser.channel);
        TEST_ASSERT_EQUAL(0, parser.byte_count);

        for (uint8_t control = 0; control < MIDI_MESSAGE_ALL_SOUND_OFF;
             control++) {
            for (uint8_t value = 0; value < 128; value++) {
                result = midi_parse_byte(&parser, control, &message);
                TEST_ASSERT_EQUAL(MIDI_MESSAGE_NONE, result);
                TEST_ASSERT_EQUAL(MIDI_MESSAGE_CONTROL_CHANGE,
                                  parser.message_type);
                TEST_ASSERT_EQUAL(channel, parser.channel);
                TEST_ASSERT_EQUAL(1, parser.byte_count);

                result = midi_parse_byte(&parser, value, &message);
                TEST_ASSERT_EQUAL(MIDI_MESSAGE_CONTROL_CHANGE, result);
                TEST_ASSERT_EQUAL(MIDI_MESSAGE_CONTROL_CHANGE,
                                  parser.message_type);
                TEST_ASSERT_EQUAL(channel, parser.channel);
                TEST_ASSERT_EQUAL(0, parser.byte_count);

                TEST_ASSERT_EQUAL(MIDI_MESSAGE_CONTROL_CHANGE,
                                  message.message_type);
                TEST_ASSERT_EQUAL(channel, message.channel);
                TEST_ASSERT_EQUAL(control, message.controller);
                TEST_ASSERT_EQUAL(value, message.control_value);
            }
        }
    }
}

/**
 * @brief Test the channel mode messages
 * @details Tests every possible channel mode message
 *          Checks that running status works correctly
 */
void test_channel_mode_messages(void)
{
    midi_message_type_t result;

    for (uint8_t channel = 0; channel < 16; channel++) {
        result = midi_parse_byte(
            &parser, MIDI_MESSAGE_CONTROL_CHANGE | channel, &message);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_NONE, result);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_CONTROL_CHANGE, parser.message_type);
        TEST_ASSERT_EQUAL(channel, parser.channel);
        TEST_ASSERT_EQUAL(0, parser.byte_count);

        for (uint8_t control = MIDI_MESSAGE_ALL_SOUND_OFF; control < 0x80;
             control++) {
            for (uint8_t value = 0; value < 128; value += 127) {
                result = midi_parse_byte(&parser, control, &message);
                TEST_ASSERT_EQUAL(MIDI_MESSAGE_NONE, result);
                TEST_ASSERT_EQUAL(MIDI_MESSAGE_CONTROL_CHANGE,
                                  parser.message_type);
                TEST_ASSERT_EQUAL(channel, parser.channel);
                TEST_ASSERT_EQUAL(1, parser.byte_count);

                result = midi_parse_byte(&parser, value, &message);
                TEST_ASSERT_EQUAL(control, result);
                TEST_ASSERT_EQUAL(MIDI_MESSAGE_CONTROL_CHANGE,
                                  parser.message_type);
                TEST_ASSERT_EQUAL(channel, parser.channel);
                TEST_ASSERT_EQUAL(0, parser.byte_count);

                TEST_ASSERT_EQUAL(control, message.message_type);
                TEST_ASSERT_EQUAL(channel, message.channel);
                TEST_ASSERT_EQUAL(control, message.controller);
                TEST_ASSERT_EQUAL(value, message.control_value);
            }
        }
    }
}

/**
 * @brief Test the program change message
 * @details Tests every possible program change message
 *          Checks that running status works correctly
 */
void test_program_change_message(void)
{
    midi_message_type_t result;

    for (uint8_t channel = 0; channel < 16; channel++) {
        result = midi_parse_byte(
            &parser, MIDI_MESSAGE_PROGRAM_CHANGE | channel, &message);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_NONE, result);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_PROGRAM_CHANGE, parser.message_type);
        TEST_ASSERT_EQUAL(channel, parser.channel);
        TEST_ASSERT_EQUAL(0, parser.byte_count);

        for (uint8_t program = 0; program < 128; program++) {
            result = midi_parse_byte(&parser, program, &message);
            TEST_ASSERT_EQUAL(MIDI_MESSAGE_PROGRAM_CHANGE, result);
            TEST_ASSERT_EQUAL(MIDI_MESSAGE_PROGRAM_CHANGE, parser.message_type);
            TEST_ASSERT_EQUAL(channel, parser.channel);
            TEST_ASSERT_EQUAL(0, parser.byte_count);

            TEST_ASSERT_EQUAL(MIDI_MESSAGE_PROGRAM_CHANGE,
                              message.message_type);
            TEST_ASSERT_EQUAL(channel, message.channel);
            TEST_ASSERT_EQUAL(program, message.program);
        }
    }
}

/**
 * @brief Test the channel pressure message
 * @details Tests every possible channel pressure message
 */
void test_channel_pressure_message(void)
{
    midi_message_type_t result;

    for (uint8_t channel = 0; channel < 16; channel++) {
        result = midi_parse_byte(
            &parser, MIDI_MESSAGE_CHANNEL_PRESSURE | channel, &message);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_NONE, result);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_CHANNEL_PRESSURE, parser.message_type);
        TEST_ASSERT_EQUAL(channel, parser.channel);
        TEST_ASSERT_EQUAL(0, parser.byte_count);

        for (uint8_t pressure = 0; pressure < 128; pressure++) {
            result = midi_parse_byte(&parser, pressure, &message);
            TEST_ASSERT_EQUAL(MIDI_MESSAGE_CHANNEL_PRESSURE, result);
            TEST_ASSERT_EQUAL(MIDI_MESSAGE_CHANNEL_PRESSURE,
                              parser.message_type);
            TEST_ASSERT_EQUAL(channel, parser.channel);
            TEST_ASSERT_EQUAL(0, parser.byte_count);

            TEST_ASSERT_EQUAL(MIDI_MESSAGE_CHANNEL_PRESSURE,
                              message.message_type);
            TEST_ASSERT_EQUAL(channel, message.channel);
            TEST_ASSERT_EQUAL(pressure, message.channel_pressure);
        }
    }
}

/**
 * @brief Test the pitch bend message
 * @details Tests every possible pitch bend message
 *          Checks that running status works correctly
 */
void test_pitch_bend_message(void)
{
    midi_message_type_t result;
    for (uint8_t channel = 0; channel < 16; channel++) {
        result = midi_parse_byte(
            &parser, MIDI_MESSAGE_PITCH_BEND | channel, &message);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_NONE, result);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_PITCH_BEND, parser.message_type);
        TEST_ASSERT_EQUAL(channel, parser.channel);
        TEST_ASSERT_EQUAL(0, parser.byte_count);

        for (uint16_t bend = 0; bend < 16384; bend++) {
            result = midi_parse_byte(&parser, bend & 0x7F, &message);
            TEST_ASSERT_EQUAL(MIDI_MESSAGE_NONE, result);
            TEST_ASSERT_EQUAL(MIDI_MESSAGE_PITCH_BEND, parser.message_type);
            TEST_ASSERT_EQUAL(channel, parser.channel);
            TEST_ASSERT_EQUAL(1, parser.byte_count);

            result = midi_parse_byte(&parser, bend >> 7, &message);
            TEST_ASSERT_EQUAL(MIDI_MESSAGE_PITCH_BEND, result);
            TEST_ASSERT_EQUAL(MIDI_MESSAGE_PITCH_BEND, parser.message_type);
            TEST_ASSERT_EQUAL(channel, parser.channel);
            TEST_ASSERT_EQUAL(0, parser.byte_count);

            TEST_ASSERT_EQUAL(MIDI_MESSAGE_PITCH_BEND, message.message_type);
            TEST_ASSERT_EQUAL(channel, message.channel);
            TEST_ASSERT_EQUAL(bend, message.pitch_bend);
        }
    }
}

/*=====================================================================*
    System Messages
 *=====================================================================*/

/**
 * @brief Test the MTC quarter frame message
 * @details Tests every possible MTC quarter frame message
 */
void test_mtc_quarter_frame_message(void)
{
    midi_message_type_t result;

    for (uint8_t data = 0; data < 128; data++) {
        result = midi_parse_byte(
            &parser, MIDI_MESSAGE_MTC_QUARTER_FRAME, &message);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_NONE, result);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_MTC_QUARTER_FRAME, parser.message_type);
        TEST_ASSERT_EQUAL(0, parser.byte_count);

        result = midi_parse_byte(&parser, data, &message);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_MTC_QUARTER_FRAME, result);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_NONE, parser.message_type);
        TEST_ASSERT_EQUAL(MIDI_CHANNEL_NONE, parser.channel);
        TEST_ASSERT_EQUAL(0, parser.byte_count);

        TEST_ASSERT_EQUAL(MIDI_MESSAGE_MTC_QUARTER_FRAME, message.message_type);
        TEST_ASSERT_EQUAL(MIDI_CHANNEL_NONE, message.channel);
        TEST_ASSERT_EQUAL(data >> 4, message.mtc_msg_type);
        TEST_ASSERT_EQUAL(data & 0x0F, message.mtc_values);
    }
}

/**
 * @brief Test the song position pointer message
 * @details Tests every possible song position pointer message
 */
void test_song_position_pointer_message(void)
{
    midi_message_type_t result;

    for (uint16_t spp = 0; spp < 16384; spp++) {

        result = midi_parse_byte(
            &parser, MIDI_MESSAGE_SONG_POSITION_POINTER, &message);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_NONE, result);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_SONG_POSITION_POINTER,
                          parser.message_type);
        TEST_ASSERT_EQUAL(MIDI_CHANNEL_NONE, parser.channel);
        TEST_ASSERT_EQUAL(0, parser.byte_count);

        result = midi_parse_byte(&parser, spp & 0x7F, &message);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_NONE, result);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_SONG_POSITION_POINTER,
                          parser.message_type);
        TEST_ASSERT_EQUAL(MIDI_CHANNEL_NONE, parser.channel);
        TEST_ASSERT_EQUAL(1, parser.byte_count);

        result = midi_parse_byte(&parser, spp >> 7, &message);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_SONG_POSITION_POINTER, result);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_NONE, parser.message_type);
        TEST_ASSERT_EQUAL(MIDI_CHANNEL_NONE, parser.channel);
        TEST_ASSERT_EQUAL(0, parser.byte_count);

        TEST_ASSERT_EQUAL(MIDI_MESSAGE_SONG_POSITION_POINTER,
                          message.message_type);
        TEST_ASSERT_EQUAL(MIDI_CHANNEL_NONE, message.channel);
        TEST_ASSERT_EQUAL(spp, message.song_position);
    }
}

/**
 * @brief Test the song select message
 * @details Tests every possible song select message
 */
void test_song_select_message(void)
{
    midi_message_type_t result;

    for (uint16_t song = 0; song < 128; song++) {

        result = midi_parse_byte(&parser, MIDI_MESSAGE_SONG_SELECT, &message);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_NONE, result);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_SONG_SELECT, parser.message_type);
        TEST_ASSERT_EQUAL(MIDI_CHANNEL_NONE, parser.channel);
        TEST_ASSERT_EQUAL(0, parser.byte_count);

        result = midi_parse_byte(&parser, song, &message);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_SONG_SELECT, result);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_NONE, parser.message_type);
        TEST_ASSERT_EQUAL(MIDI_CHANNEL_NONE, parser.channel);
        TEST_ASSERT_EQUAL(0, parser.byte_count);

        TEST_ASSERT_EQUAL(MIDI_MESSAGE_SONG_SELECT, message.message_type);
        TEST_ASSERT_EQUAL(MIDI_CHANNEL_NONE, message.channel);
        TEST_ASSERT_EQUAL(song, message.song_select);
    }
}

/**
 * @brief Test the tune request message
 */
void test_tune_request_message(void)
{
    midi_message_type_t result;

    result = midi_parse_byte(&parser, MIDI_MESSAGE_TUNE_REQUEST, &message);
    TEST_ASSERT_EQUAL(MIDI_MESSAGE_TUNE_REQUEST, result);
    TEST_ASSERT_EQUAL(MIDI_MESSAGE_NONE, parser.message_type);
    TEST_ASSERT_EQUAL(MIDI_CHANNEL_NONE, parser.channel);
    TEST_ASSERT_EQUAL(0, parser.byte_count);

    TEST_ASSERT_EQUAL(MIDI_MESSAGE_TUNE_REQUEST, message.message_type);
    TEST_ASSERT_EQUAL(MIDI_CHANNEL_NONE, message.channel);
}

/**
 * @brief Test the timing clock message
 */
void test_timing_clock_message(void)
{
    midi_message_type_t result;

    result = midi_parse_byte(&parser, MIDI_MESSAGE_TIMING_CLOCK, &message);
    TEST_ASSERT_EQUAL(MIDI_MESSAGE_TIMING_CLOCK, result);
    TEST_ASSERT_EQUAL(MIDI_MESSAGE_NONE, parser.message_type);
    TEST_ASSERT_EQUAL(MIDI_CHANNEL_NONE, parser.channel);
    TEST_ASSERT_EQUAL(0, parser.byte_count);

    TEST_ASSERT_EQUAL(MIDI_MESSAGE_TIMING_CLOCK, message.message_type);
    TEST_ASSERT_EQUAL(MIDI_CHANNEL_NONE, message.channel);
}

/**
 * @brief Test the start message
 */
void test_start_message(void)
{
    midi_message_type_t result;

    result = midi_parse_byte(&parser, MIDI_MESSAGE_START, &message);
    TEST_ASSERT_EQUAL(MIDI_MESSAGE_START, result);
    TEST_ASSERT_EQUAL(MIDI_MESSAGE_NONE, parser.message_type);
    TEST_ASSERT_EQUAL(MIDI_CHANNEL_NONE, parser.channel);
    TEST_ASSERT_EQUAL(0, parser.byte_count);

    TEST_ASSERT_EQUAL(MIDI_MESSAGE_START, message.message_type);
    TEST_ASSERT_EQUAL(MIDI_CHANNEL_NONE, message.channel);
}

/**
 * @brief Test the continue message
 */
void test_continue_message(void)
{
    midi_message_type_t result;

    result = midi_parse_byte(&parser, MIDI_MESSAGE_CONTINUE, &message);
    TEST_ASSERT_EQUAL(MIDI_MESSAGE_CONTINUE, result);
    TEST_ASSERT_EQUAL(MIDI_MESSAGE_NONE, parser.message_type);
    TEST_ASSERT_EQUAL(MIDI_CHANNEL_NONE, parser.channel);
    TEST_ASSERT_EQUAL(0, parser.byte_count);

    TEST_ASSERT_EQUAL(MIDI_MESSAGE_CONTINUE, message.message_type);
    TEST_ASSERT_EQUAL(MIDI_CHANNEL_NONE, message.channel);
}

/**
 * @brief Test the stop message
 */
void test_stop_message(void)
{
    midi_message_type_t result;

    result = midi_parse_byte(&parser, MIDI_MESSAGE_STOP, &message);
    TEST_ASSERT_EQUAL(MIDI_MESSAGE_STOP, result);
    TEST_ASSERT_EQUAL(MIDI_MESSAGE_NONE, parser.message_type);
    TEST_ASSERT_EQUAL(MIDI_CHANNEL_NONE, parser.channel);
    TEST_ASSERT_EQUAL(0, parser.byte_count);

    TEST_ASSERT_EQUAL(MIDI_MESSAGE_STOP, message.message_type);
    TEST_ASSERT_EQUAL(MIDI_CHANNEL_NONE, message.channel);
}

/**
 * @brief Test the active sense message
 */
void test_active_sense_message(void)
{
    midi_message_type_t result;

    result = midi_parse_byte(&parser, MIDI_MESSAGE_ACTIVE_SENSE, &message);
    TEST_ASSERT_EQUAL(MIDI_MESSAGE_ACTIVE_SENSE, result);
    TEST_ASSERT_EQUAL(MIDI_MESSAGE_NONE, parser.message_type);
    TEST_ASSERT_EQUAL(MIDI_CHANNEL_NONE, parser.channel);
    TEST_ASSERT_EQUAL(0, parser.byte_count);

    TEST_ASSERT_EQUAL(MIDI_MESSAGE_ACTIVE_SENSE, message.message_type);
    TEST_ASSERT_EQUAL(MIDI_CHANNEL_NONE, message.channel);
}

/**
 * @brief Test the system reset message
 */
void test_system_reset_message(void)
{
    midi_message_type_t result;

    result = midi_parse_byte(&parser, MIDI_MESSAGE_SYSTEM_RESET, &message);
    TEST_ASSERT_EQUAL(MIDI_MESSAGE_SYSTEM_RESET, result);
    TEST_ASSERT_EQUAL(MIDI_MESSAGE_NONE, parser.message_type);
    TEST_ASSERT_EQUAL(MIDI_CHANNEL_NONE, parser.channel);
    TEST_ASSERT_EQUAL(0, parser.byte_count);

    TEST_ASSERT_EQUAL(MIDI_MESSAGE_SYSTEM_RESET, message.message_type);
    TEST_ASSERT_EQUAL(MIDI_CHANNEL_NONE, message.channel);
}

/*=====================================================================*
    Sysex Messages
 *=====================================================================*/

/**
 * @brief Test the sysex message
 */
void test_sysex_message(void)
{
    midi_message_type_t result;

    // Send SysEx start
    result = midi_parse_byte(&parser, MIDI_MESSAGE_SYSTEM_EXCLUSIVE, &message);
    TEST_ASSERT_EQUAL(MIDI_MESSAGE_SYSTEM_EXCLUSIVE, result);
    TEST_ASSERT_EQUAL(MIDI_MESSAGE_SYSTEM_EXCLUSIVE, parser.message_type);
    TEST_ASSERT_EQUAL(MIDI_CHANNEL_NONE, parser.channel);
    TEST_ASSERT_EQUAL(0, parser.byte_count);

    TEST_ASSERT_EQUAL(MIDI_MESSAGE_SYSTEM_EXCLUSIVE, message.message_type);
    TEST_ASSERT_EQUAL(MIDI_CHANNEL_NONE, message.channel);

    // Send SysEx data
    for (uint8_t i = 0; i < 10; i++) {
        result = midi_parse_byte(&parser, i, &message);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_NONE, result);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_SYSTEM_EXCLUSIVE, parser.message_type);
        TEST_ASSERT_EQUAL(MIDI_CHANNEL_NONE, parser.channel);
        TEST_ASSERT_EQUAL(0, parser.byte_count);
    }

    // Send SysEx end
    result = midi_parse_byte(&parser, MIDI_MESSAGE_END_OF_EXCLUSIVE, &message);
    TEST_ASSERT_EQUAL(MIDI_MESSAGE_END_OF_EXCLUSIVE, result);
    TEST_ASSERT_EQUAL(MIDI_MESSAGE_NONE, parser.message_type);
    TEST_ASSERT_EQUAL(MIDI_CHANNEL_NONE, parser.channel);
    TEST_ASSERT_EQUAL(0, parser.byte_count);

    TEST_ASSERT_EQUAL(MIDI_MESSAGE_END_OF_EXCLUSIVE, message.message_type);
    TEST_ASSERT_EQUAL(MIDI_CHANNEL_NONE, message.channel);
}

/*=====================================================================*
    Interruptions

    Multi-byte messages are allowed to be interrupted
    by System Real-Time messages.

    Undefined status bytes should be ignored without causing any
    disruption to the parser.
 *=====================================================================*/

/**
 * @brief Interrupt a channel message with system realtime messages
 */
void test_interrupt_channel_message_with_system_realtime_messages(void)
{
    uint8_t realtime_bytes[] = {MIDI_MESSAGE_TIMING_CLOCK,
                                MIDI_MESSAGE_START,
                                MIDI_MESSAGE_CONTINUE,
                                MIDI_MESSAGE_STOP,
                                MIDI_MESSAGE_ACTIVE_SENSE,
                                MIDI_MESSAGE_SYSTEM_RESET};
    midi_message_type_t result;

    for (uint8_t i = 0; i < sizeof(realtime_bytes); i++) {

        // Start to send a note on message
        result = midi_parse_byte(&parser, MIDI_MESSAGE_NOTE_ON, &message);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_NONE, result);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_NOTE_ON, parser.message_type);
        TEST_ASSERT_EQUAL(MIDI_CHANNEL_1, parser.channel);
        TEST_ASSERT_EQUAL(0, parser.byte_count);

        // Send a note number
        result = midi_parse_byte(&parser, 60, &message);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_NONE, result);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_NOTE_ON, parser.message_type);
        TEST_ASSERT_EQUAL(MIDI_CHANNEL_1, parser.channel);
        TEST_ASSERT_EQUAL(1, parser.byte_count);

        // Send a realtime message
        result = midi_parse_byte(&parser, realtime_bytes[i], &message);
        TEST_ASSERT_EQUAL(realtime_bytes[i], result);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_NOTE_ON, parser.message_type);
        TEST_ASSERT_EQUAL(MIDI_CHANNEL_1, parser.channel);
        TEST_ASSERT_EQUAL(1, parser.byte_count);

        // Send the last note on byte and check that it is parsed correctly
        result = midi_parse_byte(&parser, 100, &message);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_NOTE_ON, result);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_NOTE_ON, parser.message_type);
        TEST_ASSERT_EQUAL(MIDI_CHANNEL_1, parser.channel);
        TEST_ASSERT_EQUAL(0, parser.byte_count);

        TEST_ASSERT_EQUAL(MIDI_MESSAGE_NOTE_ON, message.message_type);
        TEST_ASSERT_EQUAL(MIDI_CHANNEL_1, message.channel);
        TEST_ASSERT_EQUAL(60, message.note);
        TEST_ASSERT_EQUAL(100, message.velocity);
    }
}

/**
 * @brief Interrupt a channel message with undefined status bytes
 */
void test_interrupt_channel_message_with_undefined_status_bytes(void)
{
    uint8_t undefined_bytes[] = {0xF4, 0xF5, 0xF9, 0xFD};
    midi_message_type_t result;

    for (uint8_t i = 0; i < sizeof(undefined_bytes); i++) {

        // Start to send a note on message
        result = midi_parse_byte(&parser, MIDI_MESSAGE_NOTE_ON, &message);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_NONE, result);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_NOTE_ON, parser.message_type);
        TEST_ASSERT_EQUAL(MIDI_CHANNEL_1, parser.channel);
        TEST_ASSERT_EQUAL(0, parser.byte_count);

        // Send a note number
        result = midi_parse_byte(&parser, 60, &message);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_NONE, result);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_NOTE_ON, parser.message_type);
        TEST_ASSERT_EQUAL(MIDI_CHANNEL_1, parser.channel);
        TEST_ASSERT_EQUAL(1, parser.byte_count);

        // Send undefined status bytes
        result = midi_parse_byte(&parser, undefined_bytes[i], &message);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_NONE, result);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_NOTE_ON, parser.message_type);
        TEST_ASSERT_EQUAL(MIDI_CHANNEL_1, parser.channel);
        TEST_ASSERT_EQUAL(1, parser.byte_count);

        // Send the last note on byte and check that it is parsed correctly
        result = midi_parse_byte(&parser, 100, &message);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_NOTE_ON, result);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_NOTE_ON, parser.message_type);
        TEST_ASSERT_EQUAL(MIDI_CHANNEL_1, parser.channel);
        TEST_ASSERT_EQUAL(0, parser.byte_count);

        TEST_ASSERT_EQUAL(MIDI_MESSAGE_NOTE_ON, message.message_type);
        TEST_ASSERT_EQUAL(MIDI_CHANNEL_1, message.channel);
        TEST_ASSERT_EQUAL(60, message.note);
        TEST_ASSERT_EQUAL(100, message.velocity);
    }
}

/**
 * @brief Interrupt a SysEx message with system realtime messages
 */
void test_interrupt_sysex_with_system_realtime_messages(void)
{
    uint8_t realtime_bytes[] = {MIDI_MESSAGE_TIMING_CLOCK,
                                MIDI_MESSAGE_START,
                                MIDI_MESSAGE_CONTINUE,
                                MIDI_MESSAGE_STOP,
                                MIDI_MESSAGE_ACTIVE_SENSE,
                                MIDI_MESSAGE_SYSTEM_RESET};
    midi_message_type_t result;

    for (uint8_t i = 0; i < sizeof(realtime_bytes); i++) {

        // Start to send a SysEx message
        result = midi_parse_byte(
            &parser, MIDI_MESSAGE_SYSTEM_EXCLUSIVE, &message);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_SYSTEM_EXCLUSIVE, result);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_SYSTEM_EXCLUSIVE, parser.message_type);
        TEST_ASSERT_EQUAL(MIDI_CHANNEL_NONE, parser.channel);
        TEST_ASSERT_EQUAL(0, parser.byte_count);

        TEST_ASSERT_EQUAL(MIDI_MESSAGE_SYSTEM_EXCLUSIVE, message.message_type);
        TEST_ASSERT_EQUAL(MIDI_CHANNEL_NONE, message.channel);

        // Send a SysEx data byte
        result = midi_parse_byte(&parser, 0x0A, &message);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_NONE, result);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_SYSTEM_EXCLUSIVE, parser.message_type);
        TEST_ASSERT_EQUAL(MIDI_CHANNEL_NONE, parser.channel);
        TEST_ASSERT_EQUAL(0, parser.byte_count);

        // Send a realtime message
        result = midi_parse_byte(&parser, realtime_bytes[i], &message);
        TEST_ASSERT_EQUAL(realtime_bytes[i], result);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_SYSTEM_EXCLUSIVE, parser.message_type);
        TEST_ASSERT_EQUAL(MIDI_CHANNEL_NONE, parser.channel);
        TEST_ASSERT_EQUAL(0, parser.byte_count);

        TEST_ASSERT_EQUAL(realtime_bytes[i], message.message_type);
        TEST_ASSERT_EQUAL(MIDI_CHANNEL_NONE, message.channel);

        // Send a another SysEx data byte
        result = midi_parse_byte(&parser, 0x05, &message);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_NONE, result);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_SYSTEM_EXCLUSIVE, parser.message_type);
        TEST_ASSERT_EQUAL(MIDI_CHANNEL_NONE, parser.channel);
        TEST_ASSERT_EQUAL(0, parser.byte_count);

        // Send EOX
        result = midi_parse_byte(
            &parser, MIDI_MESSAGE_END_OF_EXCLUSIVE, &message);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_END_OF_EXCLUSIVE, result);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_NONE, parser.message_type);
        TEST_ASSERT_EQUAL(MIDI_CHANNEL_NONE, parser.channel);
        TEST_ASSERT_EQUAL(0, parser.byte_count);

        TEST_ASSERT_EQUAL(MIDI_MESSAGE_END_OF_EXCLUSIVE, message.message_type);
        TEST_ASSERT_EQUAL(MIDI_CHANNEL_NONE, message.channel);
    }
}

/**
 * @brief Interrupt a SysEx message with system realtime messages
 */
void test_interrupt_sysex_with_undefined_status_bytes(void)
{
    uint8_t undefined_bytes[] = {0xF4, 0xF5, 0xF9, 0xFD};
    midi_message_type_t result;

    for (uint8_t i = 0; i < sizeof(undefined_bytes); i++) {

        // Start to send a SysEx message
        result = midi_parse_byte(
            &parser, MIDI_MESSAGE_SYSTEM_EXCLUSIVE, &message);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_SYSTEM_EXCLUSIVE, result);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_SYSTEM_EXCLUSIVE, parser.message_type);
        TEST_ASSERT_EQUAL(MIDI_CHANNEL_NONE, parser.channel);
        TEST_ASSERT_EQUAL(0, parser.byte_count);

        TEST_ASSERT_EQUAL(MIDI_MESSAGE_SYSTEM_EXCLUSIVE, message.message_type);
        TEST_ASSERT_EQUAL(MIDI_CHANNEL_NONE, message.channel);

        // Send a SysEx data byte
        result = midi_parse_byte(&parser, 0x0A, &message);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_NONE, result);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_SYSTEM_EXCLUSIVE, parser.message_type);
        TEST_ASSERT_EQUAL(MIDI_CHANNEL_NONE, parser.channel);
        TEST_ASSERT_EQUAL(0, parser.byte_count);

        // Send a undefined status byte
        result = midi_parse_byte(&parser, undefined_bytes[i], &message);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_NONE, result);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_SYSTEM_EXCLUSIVE, parser.message_type);
        TEST_ASSERT_EQUAL(MIDI_CHANNEL_NONE, parser.channel);
        TEST_ASSERT_EQUAL(0, parser.byte_count);

        // Send a another SysEx data byte
        result = midi_parse_byte(&parser, 0x05, &message);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_NONE, result);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_SYSTEM_EXCLUSIVE, parser.message_type);
        TEST_ASSERT_EQUAL(MIDI_CHANNEL_NONE, parser.channel);
        TEST_ASSERT_EQUAL(0, parser.byte_count);

        // Send EOX
        result = midi_parse_byte(
            &parser, MIDI_MESSAGE_END_OF_EXCLUSIVE, &message);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_END_OF_EXCLUSIVE, result);
        TEST_ASSERT_EQUAL(MIDI_MESSAGE_NONE, parser.message_type);
        TEST_ASSERT_EQUAL(MIDI_CHANNEL_NONE, parser.channel);
        TEST_ASSERT_EQUAL(0, parser.byte_count);

        TEST_ASSERT_EQUAL(MIDI_MESSAGE_END_OF_EXCLUSIVE, message.message_type);
        TEST_ASSERT_EQUAL(MIDI_CHANNEL_NONE, message.channel);
    }
}

/*=====================================================================*
   Partial Messages
*=====================================================================*/

/**
 * @brief Test handling of incomplete messages followed by new status bytes
 */
void test_partial_channel_message(void)
{
    midi_message_type_t result;

    // Start a note on message
    result = midi_parse_byte(&parser, MIDI_MESSAGE_NOTE_ON, &message);
    result = midi_parse_byte(&parser, 60, &message);
    TEST_ASSERT_EQUAL(MIDI_MESSAGE_NONE, result);
    TEST_ASSERT_EQUAL(1, parser.byte_count);

    // Send a new status byte before completing the message
    result = midi_parse_byte(&parser, MIDI_MESSAGE_PROGRAM_CHANGE, &message);
    TEST_ASSERT_EQUAL(MIDI_MESSAGE_NONE, result);
    TEST_ASSERT_EQUAL(MIDI_MESSAGE_PROGRAM_CHANGE, parser.message_type);
    TEST_ASSERT_EQUAL(0, parser.byte_count);

    // Complete the new message
    result = midi_parse_byte(&parser, 42, &message);
    TEST_ASSERT_EQUAL(MIDI_MESSAGE_PROGRAM_CHANGE, result);
    TEST_ASSERT_EQUAL(42, message.program);
}

/**
 * @brief Test SysEx message without proper EOX termination
 */
void test_partial_sysex_message(void)
{
    midi_message_type_t result;

    // Start SysEx
    result = midi_parse_byte(&parser, MIDI_MESSAGE_SYSTEM_EXCLUSIVE, &message);
    TEST_ASSERT_EQUAL(MIDI_MESSAGE_SYSTEM_EXCLUSIVE, result);
    TEST_ASSERT_EQUAL(MIDI_MESSAGE_SYSTEM_EXCLUSIVE, parser.message_type);
    TEST_ASSERT_EQUAL(MIDI_CHANNEL_NONE, parser.channel);
    TEST_ASSERT_EQUAL(0, parser.byte_count);

    // Send some data
    result = midi_parse_byte(&parser, 0x10, &message);
    TEST_ASSERT_EQUAL(MIDI_MESSAGE_NONE, result);
    TEST_ASSERT_EQUAL(MIDI_MESSAGE_SYSTEM_EXCLUSIVE, parser.message_type);
    TEST_ASSERT_EQUAL(MIDI_CHANNEL_NONE, parser.channel);
    TEST_ASSERT_EQUAL(0, parser.byte_count);

    result = midi_parse_byte(&parser, 0x20, &message);
    TEST_ASSERT_EQUAL(MIDI_MESSAGE_NONE, result);
    TEST_ASSERT_EQUAL(MIDI_MESSAGE_SYSTEM_EXCLUSIVE, parser.message_type);
    TEST_ASSERT_EQUAL(MIDI_CHANNEL_NONE, parser.channel);
    TEST_ASSERT_EQUAL(0, parser.byte_count);

    // Send a new status byte instead of EOX
    result = midi_parse_byte(&parser, MIDI_MESSAGE_NOTE_ON, &message);
    TEST_ASSERT_EQUAL(MIDI_MESSAGE_NONE, result);
    TEST_ASSERT_EQUAL(MIDI_MESSAGE_NOTE_ON, parser.message_type);

    // Complete the note on message
    result = midi_parse_byte(&parser, 60, &message);
    TEST_ASSERT_EQUAL(MIDI_MESSAGE_NONE, result);
    TEST_ASSERT_EQUAL(MIDI_MESSAGE_NOTE_ON, parser.message_type);
    TEST_ASSERT_EQUAL(MIDI_CHANNEL_1, parser.channel);
    TEST_ASSERT_EQUAL(1, parser.byte_count);

    // Send the last byte and check that it is parsed correctly
    result = midi_parse_byte(&parser, 100, &message);
    TEST_ASSERT_EQUAL(MIDI_MESSAGE_NOTE_ON, result);
    TEST_ASSERT_EQUAL(MIDI_MESSAGE_NOTE_ON, parser.message_type);
    TEST_ASSERT_EQUAL(MIDI_CHANNEL_1, parser.channel);
    TEST_ASSERT_EQUAL(0, parser.byte_count);

    TEST_ASSERT_EQUAL(MIDI_MESSAGE_NOTE_ON, message.message_type);
    TEST_ASSERT_EQUAL(MIDI_CHANNEL_1, message.channel);
    TEST_ASSERT_EQUAL(60, message.note);
    TEST_ASSERT_EQUAL(100, message.velocity);
}

/*=====================================================================*
    Main Test Runner
 *=====================================================================*/
int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_midi_parser_init);
    RUN_TEST(test_null_pointer_handling);

    // Channel messages
    RUN_TEST(test_note_on_message);
    RUN_TEST(test_note_off_message);
    RUN_TEST(test_poly_key_pressure);
    RUN_TEST(test_control_change_message);
    RUN_TEST(test_channel_mode_messages);
    RUN_TEST(test_program_change_message);
    RUN_TEST(test_pitch_bend_message);
    RUN_TEST(test_channel_pressure_message);

    // System messages
    RUN_TEST(test_mtc_quarter_frame_message);
    RUN_TEST(test_song_position_pointer_message);
    RUN_TEST(test_song_select_message);
    RUN_TEST(test_tune_request_message);
    RUN_TEST(test_timing_clock_message);
    RUN_TEST(test_start_message);
    RUN_TEST(test_continue_message);
    RUN_TEST(test_stop_message);
    RUN_TEST(test_active_sense_message);
    RUN_TEST(test_system_reset_message);
    RUN_TEST(test_sysex_message);

    // Interruptions
    RUN_TEST(test_interrupt_channel_message_with_system_realtime_messages);
    RUN_TEST(test_interrupt_channel_message_with_undefined_status_bytes);
    RUN_TEST(test_interrupt_sysex_with_system_realtime_messages);
    RUN_TEST(test_interrupt_sysex_with_undefined_status_bytes);

    // Partial messages
    RUN_TEST(test_partial_channel_message);
    RUN_TEST(test_partial_sysex_message);

    return UNITY_END();
}
