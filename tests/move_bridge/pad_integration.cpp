/* SPDX-License-Identifier: GPL-3.0-or-later
 * Integration regression: actual Porpoise pad backend with loopback packets.
 * Only platform Pad/UserService functions and enable-file access are stubbed.
 */
#include "../../src/porpoise_pad.hpp"
#include "../../third_party/libretro/libretro.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <array>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <ctime>
namespace ps5::debug { void mark(const char *) noexcept {} }
static unsigned native_buttons;
static bool native_axes;
extern "C" {
FILE *__real_fopen(const char *,const char *);
FILE *__wrap_fopen(const char *path,const char *mode) {
    if(!std::strcmp(path,"/data/porpoise-move-bridge.enable"))return std::tmpfile();
    return __real_fopen(path,mode);
}
int sceUserServiceInitialize(const void *){return 0;}
int sceUserServiceGetInitialUser(int *p){*p=42;return 0;}
int sceUserServiceGetLoginUserIdList(int *p){for(int i=0;i<4;i++)p[i]=-1;p[0]=42;return 0;}
int scePadInit(){return 0;}
int scePadOpen(int,int,int,const void *){return 1;}
int scePadGetHandle(int,int,int){return 1;}
int scePadRead(int,void *b,int){std::memset(b,0,120);if(native_buttons){std::memcpy(b,&native_buttons,4);int connected=1;std::memcpy((char*)b+76,&connected,4);
 if(native_axes){auto*p=(unsigned char*)b;p[4]=255;p[5]=0;p[6]=0;p[7]=255;p[8]=255;p[9]=255;float gyro=99;std::memcpy(p+40,&gyro,4);}
 }return 1;}
int scePadClose(int){return 0;}
int scePadSetVibration(int,const void *){return 0;}
int scePadSetVibrationMode(int,int){return 0;}
int scePadSetLightBar(int,const void *){return 0;}
int scePadResetLightBar(int){return 0;}
int scePadSetMotionSensorState(int,bool){return 0;}
int scePadSetTriggerEffect(int,const void *){return 0;}
int sceKernelUsleep(unsigned n){return usleep(n);}
}
static std::uint64_t now_us(){timespec t;clock_gettime(CLOCK_MONOTONIC,&t);return std::uint64_t(t.tv_sec)*1000000+t.tv_nsec/1000;}
int main(){
 using namespace porpoise::pad;
 assert(open());
 WiiConfig config;config.active=true;config.controller=WiiRemote;config.grip=GripUprightTrigger;set_wii(config);
 int fd=socket(AF_INET,SOCK_DGRAM,0);assert(fd>=0);sockaddr_in dest{};dest.sin_family=AF_INET;dest.sin_port=htons(9976);dest.sin_addr.s_addr=htonl(INADDR_LOOPBACK);
 std::uint64_t seq=0; const auto motion_start=now_us(); float gyro_x=0,gyro_y=0,gyro_z=0;unsigned char stick_x=128,stick_y=128,trigger=0;
 auto sample=[&](std::uint32_t buttons,bool connected=true,bool optical=true){
  std::array<unsigned char,184>b{};
  auto u16=[&](int at,std::uint16_t v){std::memcpy(b.data()+at,&v,2);};
  auto u32=[&](int at,std::uint32_t v){std::memcpy(b.data()+at,&v,4);};
  auto u64=[&](int at,std::uint64_t v){std::memcpy(b.data()+at,&v,8);};
  auto f32=[&](int at,float v){std::memcpy(b.data()+at,&v,4);};
  auto now=now_us();u32(0,0x3142564d);u16(4,2);u16(6,184);u64(8,seq++);u64(16,now);u32(24,connected?1:0);
  u32(32,buttons);b[36]=stick_x;b[37]=stick_y;b[38]=b[39]=128;b[40]=trigger;f32(56,1);f32(64,1);u32(108,connected?1:0);u64(112,motion_start+seq*20000);f32(72,gyro_x);f32(76,gyro_y);f32(80,gyro_z);
  if(optical){f32(152,80);f32(156,120);f32(160,9);f32(164,8);u64(168,now);u32(176,1);}
  assert(sendto(fd,b.data(),b.size(),0,reinterpret_cast<sockaddr*>(&dest),sizeof(dest))==int(b.size()));usleep(2000);
  return poll();
 };
 auto s=sample(0x20000);assert(s.connected);assert(s.buttons&BtnCross);assert(s.joypad&(1u<<RETRO_DEVICE_ID_JOYPAD_A));assert(s.motion.valid);assert(s.motion.aim_x==0);
 set_rumble(0,true,0x4000);
 std::array<unsigned char,33> feedback{};usleep(2000);
 auto got=recvfrom(fd,feedback.data(),feedback.size(),MSG_DONTWAIT,nullptr,nullptr);
 assert(got==32&&feedback[0]=='M'&&feedback[1]=='V'&&feedback[2]=='R'&&feedback[3]=='1'&&feedback[24]==64&&feedback[25]==0);
 set_rumble(0,true,0);usleep(2000);got=recvfrom(fd,feedback.data(),feedback.size(),MSG_DONTWAIT,nullptr,nullptr);
 assert(got==32&&feedback[24]==0&&feedback[25]==0);
 dest.sin_port=htons(9977);sample(0x2000);auto p2=snapshot(1);assert(p2.connected&&(p2.joypad&(1u<<RETRO_DEVICE_ID_JOYPAD_B)));
 set_rumble(1,true,0x2000);usleep(2000);sockaddr_in source{};socklen_t source_len=sizeof(source);
 got=recvfrom(fd,feedback.data(),feedback.size(),MSG_DONTWAIT,reinterpret_cast<sockaddr*>(&source),&source_len);
 assert(got==32&&source.sin_port==htons(9977)&&feedback[24]==32);
 set_rumble(1,true,0);usleep(2000);got=recvfrom(fd,feedback.data(),feedback.size(),MSG_DONTWAIT,nullptr,nullptr);assert(got==32&&feedback[24]==0);
 auto p1centres=snapshot(0).motion.centrings;sample(1);assert(!snapshot(1).joypad);assert(snapshot(0).motion.centrings==p1centres);p2=snapshot(1);
 dest.sin_port=htons(9976);s=sample(0x20000);assert(s.joypad&(1u<<RETRO_DEVICE_ID_JOYPAD_A));assert(snapshot(1).joypad==p2.joypad);
 dest.sin_port=htons(9977);sample(0,false,false);assert(!snapshot(1).connected);assert(snapshot(0).connected);
 dest.sin_port=htons(9976);
 dest.sin_port=htons(9984);sample(0x200);dest.sin_port=htons(9976);s=sample(0);assert(s.joypad&(1u<<RETRO_DEVICE_ID_JOYPAD_B));
 dest.sin_port=htons(9984);sample(0,false,false);dest.sin_port=htons(9976);s=sample(0);assert(!(s.joypad&(1u<<RETRO_DEVICE_ID_JOYPAD_B)));
 set_shot_buttons(true);s=sample(0x108000);assert(!s.screenshot);s=sample(0);set_shot_buttons(false);
 trigger=100;s=sample(0);assert(s.joypad&(1u<<RETRO_DEVICE_ID_JOYPAD_B));trigger=0;
 s=sample(0x100);assert(s.joypad&(1u<<RETRO_DEVICE_ID_JOYPAD_B));
 s=sample(0x2000);assert(s.joypad&(1u<<RETRO_DEVICE_ID_JOYPAD_B));assert(!(s.joypad&(1u<<RETRO_DEVICE_ID_JOYPAD_R2)));
 s=sample(0x4000);assert(s.joypad&(1u<<RETRO_DEVICE_ID_JOYPAD_X));assert(!(s.joypad&(1u<<RETRO_DEVICE_ID_JOYPAD_A)));
 s=sample(0x8000);assert(s.joypad&(1u<<RETRO_DEVICE_ID_JOYPAD_Y));
 s=sample(0x1000);assert(s.joypad&(1u<<RETRO_DEVICE_ID_JOYPAD_START));
 s=sample(0xf0);assert(!(s.buttons&(BtnUp|BtnDown|BtnLeft|BtnRight)));
 s=sample(0,true,false);assert(s.motion.valid&&s.motion.aim_x>=-1.0f&&s.motion.aim_x<=1.0f);
 config.controller=WiiRemoteNunchuk;set_wii(config);
 stick_x=255;stick_y=0;dest.sin_port=htons(9980);sample(0x400);stick_x=stick_y=128;dest.sin_port=htons(9976);s=sample(0);assert(s.joypad&(1u<<RETRO_DEVICE_ID_JOYPAD_X));assert(s.left_x>30000&&s.left_y < -30000);
 dest.sin_port=htons(9980);sample(0x100);dest.sin_port=htons(9976);s=sample(0);assert(s.joypad&(1u<<RETRO_DEVICE_ID_JOYPAD_Y));
 dest.sin_port=htons(9980);sample(0x6000|0xf0|2);dest.sin_port=htons(9976);s=sample(0);
 for(unsigned bit:{RETRO_DEVICE_ID_JOYPAD_A,RETRO_DEVICE_ID_JOYPAD_B,RETRO_DEVICE_ID_JOYPAD_UP,RETRO_DEVICE_ID_JOYPAD_DOWN,RETRO_DEVICE_ID_JOYPAD_LEFT,RETRO_DEVICE_ID_JOYPAD_RIGHT,RETRO_DEVICE_ID_JOYPAD_L3})assert(s.joypad&(1u<<bit));
 assert(snapshot(1).joypad==p2.joypad);
 dest.sin_port=htons(9980);sample(0);dest.sin_port=htons(9976);s=sample(0);assert(!s.joypad);
 dest.sin_port=htons(9980);sample(0,false,false);dest.sin_port=htons(9976);
 config.controller=WiiRemote;config.pointer=PointerTouch;set_wii(config);
 for(int i=0;i<50;i++)s=sample(0,true,false);
 assert(s.motion.centred);gyro_x=.08f;gyro_y=.03f;gyro_z=-.04f;
 for(int i=0;i<40;i++)s=sample(0,true,false);assert(calibrate_now(0));s=sample(0,true,false);
 assert(s.motion.gyro_bias[0]>.07f&&s.motion.gyro_bias[0]<.09f);
 gyro_x=gyro_y=gyro_z=0;
 const float before_x=s.motion.aim_x,before_y=s.motion.aim_y;
 gyro_x=.4f;gyro_y=.7f;gyro_z=.2f;
 for(int i=0;i<20;i++)s=sample(0,true,false);
 assert(s.motion.aim_x!=before_x||s.motion.aim_y!=before_y);assert(!s.joypad);
 gyro_x=gyro_y=gyro_z=0;
 s=sample(0x1,true,false);assert(!s.joypad);assert(s.motion.centred);
 s=sample(0,true,false);assert(s.motion.centred);
 assert(s.motion.aim_x>=-.95f&&s.motion.aim_x<=.95f);
 assert(s.motion.aim_y>=-.95f&&s.motion.aim_y<=.95f);
 config.active=false;set_wii(config);s=sample(0,true,false);
 assert(move_controller_active());centre_now(0);s=sample(0,true,false);
 config.active=true;set_wii(config);s=sample(0,true,false);assert(s.motion.centred);
 s=sample(0,false,false);assert(!s.connected);
 s=sample(0,true,false);assert(s.connected&&!s.motion.centred);
 for(int i=0;i<40;i++)s=sample(0,true,false);assert(s.motion.centred);
 s=sample(0,false,false);assert(!s.connected&&!s.joypad);
 config.active=false;set_wii(config);s=sample(0x10);assert(s.buttons&BtnUp);
 native_buttons=0x100008;s=sample(0);assert(s.ps_menu_combo);
 native_buttons=0x4000;s=sample(0);assert(s.buttons&BtnCross);assert(s.joypad&(1u<<RETRO_DEVICE_ID_JOYPAD_A));
 native_axes=true;native_buttons=0x4000;s=sample(0);assert(s.left_x>30000&&s.left_y<-30000);assert(s.right_x<-30000&&s.right_y>30000);assert(s.l2>30000&&s.r2>30000);assert(std::abs(s.motion.raw_gyro[0])<1);native_axes=false;

 config.active=true;config.menu=false;set_wii(config);dest.sin_port=htons(9980);sample(0x10000);assert(!consume_nav_attachment_toggle());sample(0);sample(0x10000);assert(consume_nav_attachment_toggle());assert(!consume_nav_attachment_toggle());sample(0x10000);assert(!consume_nav_attachment_toggle());sample(0);sample(0x10000);assert(consume_nav_attachment_toggle());sample(0);dest.sin_port=htons(9976);
 config.menu=true;set_wii(config);s=sample(0);assert(s.buttons&BtnCross);
 native_buttons=0;

 stick_x=255;stick_y=0;dest.sin_port=htons(9980);s=sample(0x4000);
 assert((s.buttons&(BtnCross|BtnRight|BtnUp))==(BtnCross|BtnRight|BtnUp));
 dest.sin_port=htons(9980);stick_x=stick_y=128;s=sample(0x2000);assert(s.buttons&BtnCircle);assert(!(s.buttons&BtnCross));
 s=sample(0,false,false);dest.sin_port=htons(9976);s=sample(0,false,false);
 usleep(550000);s=poll();assert(!s.connected&&!s.buttons&&!s.joypad);
 // Exercise independent inputs for all remaining player channels.
 config.active=true;config.menu=false;set_wii(config);
 for(int player=1;player<4;player++){dest.sin_port=htons(9976+player);sample(0x20000);const auto other=snapshot(player);assert(other.connected);assert(other.buttons&BtnCross);}
 porpoise::pad::close();::close(fd);puts("Porpoise bridge integration passed: Move A, Circle B, X 1, Square 2, Triangle Plus, rumble feedback, generated directions, tracking loss, disconnect, menu navigation, timeout.");
}
