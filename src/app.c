/******************************************************************************
 
 Copyright (c) 2015, Focusrite Audio Engineering Ltd.
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 
 * Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.
 
 * Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.
 
 * Neither the name of Focusrite Audio Engineering Ltd., nor the names of its
 contributors may be used to endorse or promote products derived from
 this software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
 *****************************************************************************/

//______________________________________________________________________________
//
// Headers
//______________________________________________________________________________

#include "app.h"


// Convention: store array length in array[0] for "easy" looping. Damn C.

// Array to hold the mapping
int mapping[100] = {
     100, 0, 32, 39, 34, 41, 36, 43, 38, 45, 0, // non velocity sensing keys included
    41, 36, 43, 38, 45, 40, 47, 42, 49, 44, // velsens starts with low C: 36
    45, 40, 47, 42, 49, 44, 51, 46, 53, 48,
    49, 44, 51, 46, 53, 48, 55, 50, 57, 52,
    53, 48, 55, 50, 57, 52, 59, 54, 61, 56,
    57, 52, 59, 54, 61, 56, 63, 58, 65, 60,
    61, 56, 63, 58, 65, 60, 67, 62, 69, 64,
    65, 60, 67, 62, 69, 64, 71, 66, 73, 68,
    69, 64, 71, 66, 73, 68, 75, 70, 77, 72,
    0,  68, 75, 70, 77, 72, 79, 74, 81     // there are "invisible" buttons on the corners
};

u8 no_velo[34] = {34, 0, 1, 2, 3, 4, 5, 6, 7, 8, 10, 19, 20, 29, 30, 39, 40, 49, 50, 59, 60, 69, 70, 79, 80, 89, 91, 92, 93, 94, 95, 96, 97, 98};

u8 octaves_one[4] = {4, 23, 53, 83};
u8 octaves_two[4] = {4, 16, 46, 76};

u8 default_velo = 40; // the velocity for keys without.

int int_in_array(u8 needle, u8 haystack[]){
    for(int i=1; i < haystack[0]; i++) {
        if (haystack[i] == needle) {
            return 1;
        }
    }
    return 0;
}


void reset_leds(){
    int r = 0;
    int g = 0;
    int b = MAXLED;
    
    for (int i=0; i < 100; ++i)
    {
        if (int_in_array(i, octaves_one)) {
            r = MAXLED;
            g = 0;
        } else {r=0;}
        if (int_in_array(i, octaves_two)) {
            g = MAXLED;
            r=0;
        } else {g=0;}
        if (i%2 == 0) { // even pads, inverted chord row, slightly darker.
            b = 20; 
        } else {b = MAXLED;}
        hal_plot_led(TYPEPAD, i, r, g, b);
    }
}




//______________________________________________________________________________

void app_surface_event(u8 type, u8 index, u8 value)
{
    switch (type)
    {
        case  TYPEPAD:
        {
            
            // example - send MIDI
            
            u8 velocity = value; 
            if ((int_in_array(index, no_velo)) && (value > 0)) {
                // is this pad velocity sensitive AND is it a strike
                // (as opposed to release)
                velocity = default_velo; // set the velocity to a fixed value
            }
            
            hal_send_midi(USBMIDI, NOTEON | NOTEOFF, mapping[index+1], velocity);
            if (value > 0) {
                hal_plot_led(TYPEPAD, index, MAXLED, MAXLED, MAXLED);
                for (int i = 0; i < 100; i++) {
                    if (mapping[i] == mapping[index]) { // light up same notes
                        hal_plot_led(TYPEPAD, i, MAXLED, MAXLED, MAXLED);
                    }
                }
            } else {
                reset_leds();
            }
        }
        break;
            
        case TYPESETUP:
        {
            if (value)
            {
                // save button states to flash (reload them by power cycling the hardware!)
                
            }
        }
        break;
    }
}

//______________________________________________________________________________

void app_midi_event(u8 port, u8 status, u8 d1, u8 d2)
{
    // example - MIDI interface functionality for USB "MIDI" port -> DIN port
    if (port == USBMIDI)
    {
        hal_send_midi(DINMIDI, status, d1, d2);
    }
    
    // // example -MIDI interface functionality for DIN -> USB "MIDI" port port
    if (port == DINMIDI)
    {
        hal_send_midi(USBMIDI, status, d1, d2);
    }
}

//______________________________________________________________________________

void app_sysex_event(u8 port, u8 * data, u16 count)
{
    // example - respond to UDI messages?
}

//______________________________________________________________________________

void app_aftertouch_event(u8 index, u8 value)
{
    // example - send poly aftertouch to MIDI ports
    hal_send_midi(USBMIDI, POLYAFTERTOUCH | 0, index, value);
    
    
}

//______________________________________________________________________________

void app_cable_event(u8 type, u8 value)
{
    // example - light the Setup LED to indicate cable connections
    if (type == MIDI_IN_CABLE)
    {
        hal_plot_led(TYPESETUP, 0, 0, value, 0); // green
    }
    else if (type == MIDI_OUT_CABLE)
    {
        hal_plot_led(TYPESETUP, 0, value, 0, 0); // red
    }
}

//______________________________________________________________________________

void app_timer_event()
{
    // example - send MIDI clock at 125bpm
#define TICK_MS 20
    
    static u8 ms = TICK_MS;
    
    if (++ms >= TICK_MS)
    {
        ms = 0;
        
        // send a clock pulse up the USB
        hal_send_midi(USBSTANDALONE, MIDITIMINGCLOCK, 0, 0);
    }
}

//______________________________________________________________________________

void app_init(const u16 *adc_raw)
{

    reset_leds();
}

