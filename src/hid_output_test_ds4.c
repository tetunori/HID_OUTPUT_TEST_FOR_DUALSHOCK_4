/*

HID OUTPUT Test for DualShock 4.

The MIT License (MIT)

Copyright (c) 2016 Tetsunori.Nakayama

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

// ***************************************************************************
// Includes
// ***************************************************************************
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <zlib.h>

// ***************************************************************************
// Definitions
// ***************************************************************************
#define HID_OUTPUT_TEST_DS4_HIDRAW_INTERFACE_PATH ( "/dev/hidraw0" )
#define HID_OUTPUT_TEST_DS4_DATA_SIZE   ( 79 )
#define HID_OUTPUT_TEST_DS4_DATA_CRC_OFFSET   ( 75 )
#define HID_OUTPUT_TEST_DS4_EMIT_DATA_OFFSET   ( 1 )

#define PRT_PRIFIX    "[HID OUTPUT DS4 TEST] "

#define HODS4T_DEBUG_ON ( 0 )
#define HODS4T_PRT( format, ... )   printf("\033[37m" PRT_PRIFIX format"\033[m\n", ## __VA_ARGS__ )
#define HODS4T_DPRT( format, ... )  if( HODS4T_DEBUG_ON ) printf("\033[37m" PRT_PRIFIX format"\033[m\n", ## __VA_ARGS__ )
#define HODS4T_EPRT( format, ... )  printf("\033[31m" PRT_PRIFIX format"\033[m\n", ## __VA_ARGS__ )

// ***************************************************************************
// Struct
// ***************************************************************************
struct _stDS4EmitData{
    char rumbleRight;
    char rumbleLeft;
    char r;
    char g;
    char b;
    char flashOn;
    char flashOff;
};

// ***************************************************************************
// Declarations
// ***************************************************************************
int hid_output_to_ds4_test( void );
int emit_data( struct _stDS4EmitData* pstEmitData );

// ***************************************************************************
// Variables
// ***************************************************************************
const char g_base_data_buffer[ HID_OUTPUT_TEST_DS4_DATA_SIZE ] = {
    0xa2, 0x11, 0xc0, 0x05, 0xff, 0x04, 0x00,
    0x00, /* Rumble (right / weak) */
    0x00, /* Rumble (left / strong) */
    0x00, 0x00, 0x00, /* R, G, B */
    0x00, /* flash ON duration */
    0x00, /* flash OFF duration */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x02, 0x03, 0x04 /* CRC-32 */
 };

// ***************************************************************************
// Functions
// ***************************************************************************
// Test Sequence.
int hid_output_to_ds4_test( void ){
    struct _stDS4EmitData stEmitData;
    memset( &stEmitData, 0x00, sizeof( stEmitData ) );

    // Light bar color Red.
    memset( &stEmitData, 0x00, sizeof( stEmitData ) );
    stEmitData.r = 0xff;
    emit_data( &stEmitData );
    usleep( 200000 );

    // Light bar color Green.
    memset( &stEmitData, 0x00, sizeof( stEmitData ) );
    stEmitData.g = 0xff;
    emit_data( &stEmitData );
    usleep( 200000 );

    // Light bar color Blue.
    memset( &stEmitData, 0x00, sizeof( stEmitData ) );
    stEmitData.b = 0xff;
    emit_data( &stEmitData );
    usleep( 200000 );

    // Light bar color Yellow.
    memset( &stEmitData, 0x00, sizeof( stEmitData ) );
    stEmitData.r = 0xff;
    stEmitData.g = 0xff;
    emit_data( &stEmitData );
    usleep( 200000 );

    // Light bar color Magenta.
    memset( &stEmitData, 0x00, sizeof( stEmitData ) );
    stEmitData.r = 0xff;
    stEmitData.b = 0xff;
    emit_data( &stEmitData );
    usleep( 200000 );

    // Light bar color Cyan.
    memset( &stEmitData, 0x00, sizeof( stEmitData ) );
    stEmitData.g = 0xff;
    stEmitData.b = 0xff;
    emit_data( &stEmitData );
    usleep( 200000 );

    // Light bar color White.
    memset( &stEmitData, 0x00, sizeof( stEmitData ) );
    stEmitData.r = 0xff;
    stEmitData.g = 0xff;
    stEmitData.b = 0xff;
    emit_data( &stEmitData );
    usleep( 500000 );

    // Flash Light bar
    memset( &stEmitData, 0x00, sizeof( stEmitData ) );
    stEmitData.g = 0xff;
    stEmitData.flashOn = 0x30;
    stEmitData.flashOff = 0x30;
    emit_data( &stEmitData );
    usleep( 2000000 );

    // All Off.
    memset( &stEmitData, 0x00, sizeof( stEmitData ) );
    emit_data( &stEmitData );

    // rumbleRight
    memset( &stEmitData, 0x00, sizeof( stEmitData ) );
    stEmitData.rumbleRight = 0x40;
    stEmitData.r = 0x30;
    emit_data( &stEmitData );
    usleep( 1000000 );
    stEmitData.rumbleRight = 0x80;
    stEmitData.r = 0x40;
    emit_data( &stEmitData );
    usleep( 1000000 );
    stEmitData.rumbleRight = 0xC0;
    stEmitData.r = 0x50;
    emit_data( &stEmitData );
    usleep( 1000000 );
    stEmitData.r = 0x60;
    stEmitData.rumbleRight = 0xff;
    emit_data( &stEmitData );
    usleep( 1000000 );

    // rumbleLeft
    memset( &stEmitData, 0x00, sizeof( stEmitData ) );
    stEmitData.rumbleLeft = 0x40;
    stEmitData.r = 0x70;
    emit_data( &stEmitData );
    usleep( 1000000 );
    stEmitData.rumbleLeft = 0x80;
    stEmitData.r = 0x80;
    emit_data( &stEmitData );
    usleep( 1000000 );
    stEmitData.rumbleLeft = 0xC0;
    stEmitData.r = 0x90;
    emit_data( &stEmitData );
    usleep( 1000000 );
    stEmitData.rumbleLeft = 0xff;
    stEmitData.r = 0xA0;
    emit_data( &stEmitData );
    usleep( 1000000 );

    // rumbleLeft & rumbleRight
    memset( &stEmitData, 0x00, sizeof( stEmitData ) );
    stEmitData.rumbleLeft = 0x40;
    stEmitData.rumbleRight = 0x40;
    stEmitData.r = 0xC0;
    emit_data( &stEmitData );
    usleep( 1000000 );
    stEmitData.rumbleLeft = 0x80;
    stEmitData.rumbleRight = 0x80;
    stEmitData.r = 0xD0;
    emit_data( &stEmitData );
    usleep( 1000000 );
    stEmitData.rumbleLeft = 0xC0;
    stEmitData.rumbleRight = 0xC0;
    stEmitData.r = 0xE0;
    emit_data( &stEmitData );
    usleep( 1000000 );
    stEmitData.rumbleLeft = 0xff;
    stEmitData.rumbleRight = 0xff;
    stEmitData.r = 0xFF;
    emit_data( &stEmitData );
    usleep( 1000000 );

    // All Off.
    memset( &stEmitData, 0x00, sizeof( stEmitData ) );
    emit_data( &stEmitData );

    return 0;
}

// Emit data through the HID pipe
int emit_data( struct _stDS4EmitData* pstEmitData )
{
    int fd = 0;
    int res = 0;
    char send_data_buf[ HID_OUTPUT_TEST_DS4_DATA_SIZE ];

    // Open HID RAW node.
    fd = open( HID_OUTPUT_TEST_DS4_HIDRAW_INTERFACE_PATH,
                O_RDWR|O_NONBLOCK );

    if ( fd < 0 ) {
        HODS4T_EPRT( "Unable to open %s.", HID_OUTPUT_TEST_DS4_HIDRAW_INTERFACE_PATH );
        return -1;
    }

    // Overwrite data.
    memcpy( send_data_buf, g_base_data_buffer, HID_OUTPUT_TEST_DS4_DATA_SIZE );
    send_data_buf[ 7] = pstEmitData->rumbleRight;
    send_data_buf[ 8] = pstEmitData->rumbleLeft;
    send_data_buf[ 9] = pstEmitData->r;
    send_data_buf[10] = pstEmitData->g;
    send_data_buf[11] = pstEmitData->b;
    send_data_buf[12] = pstEmitData->flashOn;
    send_data_buf[13] = pstEmitData->flashOff;

    // Calcurate CRC-32.
    uLong ulong_crc = 0L;
    ulong_crc = crc32( ulong_crc, (const Bytef *)send_data_buf, HID_OUTPUT_TEST_DS4_DATA_SIZE - sizeof( uLong ) );
    HODS4T_DPRT( "CRC32: 0x%08X", (unsigned int)ulong_crc );

    send_data_buf[ HID_OUTPUT_TEST_DS4_DATA_CRC_OFFSET ]     = ulong_crc & 0xFF;
    send_data_buf[ HID_OUTPUT_TEST_DS4_DATA_CRC_OFFSET + 1 ] = ( ulong_crc >> 8 ) & 0xFF;
    send_data_buf[ HID_OUTPUT_TEST_DS4_DATA_CRC_OFFSET + 2 ] = ( ulong_crc >> 16 ) & 0xFF;
    send_data_buf[ HID_OUTPUT_TEST_DS4_DATA_CRC_OFFSET + 3 ] = ( ulong_crc >> 24 ) & 0xFF;

    HODS4T_DPRT( "CRC32: 0x%02X, 0x%02X, 0x%02X, 0x%02X.",
        send_data_buf[ HID_OUTPUT_TEST_DS4_DATA_CRC_OFFSET ] & 0xFF,
        send_data_buf[ HID_OUTPUT_TEST_DS4_DATA_CRC_OFFSET + 1 ] & 0xFF,
        send_data_buf[ HID_OUTPUT_TEST_DS4_DATA_CRC_OFFSET + 2 ] & 0xFF,
        send_data_buf[ HID_OUTPUT_TEST_DS4_DATA_CRC_OFFSET + 3 ] & 0xFF );

    // Send data to the Device
    res = write( fd, send_data_buf + HID_OUTPUT_TEST_DS4_EMIT_DATA_OFFSET,
                    HID_OUTPUT_TEST_DS4_DATA_SIZE - HID_OUTPUT_TEST_DS4_EMIT_DATA_OFFSET );
    if ( res < 0 ) {
        HODS4T_EPRT( "Error: %d", errno );
        close( fd );
        return -1;
    } else {
        HODS4T_DPRT( "write() wrote %d bytes", res );
    }

    close( fd );
    return 0;
}

// ***************************************************************************
// Main Looooooooooooop
// ***************************************************************************
int main( int argc __attribute__((__unused__)), char **argv __attribute__((__unused__)) ){

    int err, ret = 0;

    err = hid_output_to_ds4_test();
    if( err ){
        HODS4T_EPRT( "emit_data_to_ds4() error: %d", err );
        ret = -1;
        goto SUB_RET;
    }

SUB_RET:
    return ret;

}
