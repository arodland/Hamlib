/*
 * hamlib - (C) Frank Singleton 2000 (vk3fcs@@ix.netcom.com)
 *
 * ft757gx.h - (C) Frank Singleton 2000 (vk3fcs@@ix.netcom.com)
 * This shared library provides an API for communicating
 * via serial interface to an FT-757GX using the "CAT" interface
 * box (FIF-232C) or similar (max232 + some capacitors :-)
 *
 *
 *    $Id: ft757gx.h,v 1.1 2004-04-24 13:01:00 fillods Exp $  
 *
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 *
 */


#ifndef _FT757GX_H
#define _FT757GX_H 1

#define FT757GX_STATUS_UPDATE_DATA_LENGTH      345

#define FT757GX_PACING_INTERVAL                5 
#define FT757GX_PACING_DEFAULT_VALUE           0 
#define FT757GX_WRITE_DELAY                    50


/* Sequential fast writes confuse my FT757GX without this delay */

#define FT757GX_POST_WRITE_DELAY               5


/* Rough safe value for default timeout */

#define FT757GX_DEFAULT_READ_TIMEOUT  345 * ( 3 + (FT757GX_PACING_INTERVAL * FT757GX_PACING_DEFAULT_VALUE))


/*
 * 8N2 and 1 start bit = 11 bits at 4800 bps => effective byte rate = 1 byte in 2.2917 msec
 * => 345 bytes in 790 msec
 *
 * delay for 1 byte = 2.2917 + (pace_interval * 5) 
 *
 * pace_interval          time to read 345 bytes
 * ------------           ----------------------
 *
 *     0                       790 msec           
 *     1                       2515 msec
 *     2                       4240 msec
 *    255                      441 sec => 7 min 21 seconds
 *
 */



/*
 * Native FT757GX functions. This is what I have to work with :-)
 *
 */

enum ft757gx_native_cmd_e {
  FT_757GX_NATIVE_SPLIT_OFF = 0,
  FT_757GX_NATIVE_SPLIT_ON,
  FT_757GX_NATIVE_RECALL_MEM,
  FT_757GX_NATIVE_VFO_TO_MEM,
  FT_757GX_NATIVE_DLOCK_OFF,
  FT_757GX_NATIVE_DLOCK_ON,
  FT_757GX_NATIVE_VFO_A,
  FT_757GX_NATIVE_VFO_B,
  FT_757GX_NATIVE_M_TO_VFO,
  FT_757GX_NATIVE_UP_500K,
  FT_757GX_NATIVE_DOWN_500K,
  FT_757GX_NATIVE_CLARIFY_OFF,
  FT_757GX_NATIVE_CLARIFY_ON,
  FT_757GX_NATIVE_FREQ_SET,
  FT_757GX_NATIVE_MODE_SET_LSB,
  FT_757GX_NATIVE_MODE_SET_USB,
  FT_757GX_NATIVE_MODE_SET_CWW,
  FT_757GX_NATIVE_MODE_SET_CWN,
  FT_757GX_NATIVE_MODE_SET_AMW,
  FT_757GX_NATIVE_MODE_SET_AMN,
  FT_757GX_NATIVE_MODE_SET_FMW,
  FT_757GX_NATIVE_MODE_SET_FMN,
  FT_757GX_NATIVE_PACING,
  FT_757GX_NATIVE_PTT_OFF,
  FT_757GX_NATIVE_PTT_ON,
  FT_757GX_NATIVE_UPDATE,
  FT_757GX_NATIVE_SIZE		/* end marker, value indicates number of */
				/* native cmd entries */

};

typedef enum ft757gx_native_cmd_e ft757gx_native_cmd_t;



/* Internal MODES - when setting modes via cmd_mode_set() */

#define MODE_SET_LSB    0x00
#define MODE_SET_USB    0x01
#define MODE_SET_CWW    0x02
#define MODE_SET_CWN    0x03
#define MODE_SET_AMW    0x04
#define MODE_SET_AMN    0x05
#define MODE_SET_FMW    0x06
#define MODE_SET_FMN    0x07


/*
 * Mode Bitmap. Bits 5 and 6 unused
 * When READING modes
 */

#define MODE_FM     0x01
#define MODE_AM     0x02
#define MODE_CW     0x04
#define MODE_FMN    0x81
#define MODE_AMN    0x82
#define MODE_CWN    0x84
#define MODE_USB    0x08
#define MODE_LSB    0x10
#define MODE_NAR    0x80   

/* All relevent bits */
#define MODE_MASK   0x9f   


/*
 * Status Flag Masks when reading
 */

#define SF_DLOCK   0x01
#define SF_SPLIT   0x02
#define SF_CLAR    0x04
#define SF_VFOAB   0x08
#define SF_VFOMR   0x10
#define SF_RXTX    0x20
#define SF_RESV    0x40
#define SF_PRI     0x80


/*
 * Local VFO CMD's, according to spec
 */

#define FT757GX_VFO_A                  0x00
#define FT757GX_VFO_B                  0x01


/*
 * Some useful offsets in the status update map (offset) 
 *
 * Manual appears to be full of mistakes regarding offsets etc.. -- FS
 *
 */

#define FT757GX_SUMO_DISPLAYED_MODE             0x18    
#define FT757GX_SUMO_DISPLAYED_STATUS           0x00    
#define FT757GX_SUMO_DISPLAYED_FREQ             0x01    
#define FT757GX_SUMO_VFO_A_FREQ                 0x09
#define FT757GX_SUMO_VFO_B_FREQ                 0x11
    


/* 
 * API local implementation 
 */

int ft757gx_init(RIG *rig);
int ft757gx_cleanup(RIG *rig);
int ft757gx_open(RIG *rig);
int ft757gx_close(RIG *rig);

int ft757gx_set_freq(RIG *rig, vfo_t vfo, freq_t freq);
int ft757gx_get_freq(RIG *rig, vfo_t vfo, freq_t *freq);

int ft757gx_set_mode(RIG *rig, vfo_t vfo, rmode_t mode, pbwidth_t width); /* select mode */
int ft757gx_get_mode(RIG *rig, vfo_t vfo, rmode_t *mode, pbwidth_t *width); /* get mode */

int ft757gx_set_vfo(RIG *rig, vfo_t vfo); /* select vfo */
int ft757gx_get_vfo(RIG *rig, vfo_t *vfo); /* get vfo */

int ft757gx_set_ptt(RIG *rig, vfo_t vfo, ptt_t ptt);
int ft757gx_get_ptt(RIG *rig, vfo_t vfo, ptt_t *ptt);



/*
 * Below is leftovers of old interface. TODO
 *
 */

#if 0
/*
 * Allow TX commands to be disabled
 *
 */

#undef TX_ENABLED



/*
 * Mode Bitmap. Bits 5 and 6 unused
 * When reading modes
 */

#define MODE_FM     0x01
#define MODE_AM     0x02
#define MODE_CW     0x04
#define MODE_FMN    0x81
#define MODE_AMN    0x82
#define MODE_CWN    0x84
#define MODE_USB    0x08
#define MODE_LSB    0x10
#define MODE_NAR    0x80	/* narrow bit set only */

/*
 * Map band data value to band.
 *
 * Band "n" is from band_data[n] to band_data[n+1]
 */

const float band_data[11] = { 0.0, 0.1, 2.5, 4.0, 7.5, 10.5, 14.5, 18.5, 21.5, 25.0, 30.0 };


#endif


#endif /* _FT757GX_H */