#ifndef __CUST_SKY_DS_H__
#define __CUST_SKY_DS_H__


/*****************************************************
    SKY Android 모델 공통 적용사항
    Feature Name : T_SKY_MODEL_TARGET_COMMON
******************************************************/
#ifdef T_SKY_MODEL_TARGET_COMMON

#endif /* T_SKY_MODEL_TARGET_COMMON */


/* ######################################################################### */
/*****************************************************
    SKT/KT 공통 적용사항
    Feature Name : T_SKY_MODEL_TARGET_WCDMA
******************************************************/
#ifdef T_SKY_MODEL_TARGET_WCDMA
/************************************************************************************************************************
**    1. Related 3G-connection
************************************************************************************************************************/

/* 
 - Phone Interface를 접근 못하는 APP를 위하여 AIDL을 추가
 - aidl 선언 : ISkyDataConnection.aidl
 - 인터페이스 구현 : MMDataConnectionTracker.java
 - 추가 구현 파일 : DataPhone.java, Phone.java , SkyDataConInterfaceManager.java
 - aidl 서비스 추가 : service_manager.c에 .aidl 추가
 - make 파일 수정 : android/framework/base/Android.mk 수정
 - telephony/java/com/android/internal/telephony/ISkyDataConnection.aidl 추가
*/
#define FEATURE_SKY_DS_ADD_DATA_AIDL

/* 
 - startUsingNetworkFeature에서 Reconnect 호출시 Fail이 발생하여도 Phone.APN_REQUEST_STARTED을 리턴하여 Application에서 혼동을 제공함
 - reconnect 실패시 APN_REQUEST_FAILED를 리턴하도록 수정
*/
#define FEATURE_SKY_DS_BUG_FIX_STARTUSINGNETWORKFEATURE

/*
- 간헐적으로 dataConnectionChanged Action에서 저장되는 Network Info의 isAvailable 값이 false로 저장되어 있는 경우 발생
- 위 조건은 1.sim, 2.roaming, 3.서비스 상태, 4.PDP 실패 상태 (Default Enable , DISCONNECTED)를 Check하는데 
   4번 조건의 경우 NATE 등에서 PDP를 다시 Activate를 시키는 방안이 사용자 편의를 위해 좋으므로 조건 변경 
*/
#define FEATURE_SKY_DS_CHANGE_IN_AVAILABLE_CHECK_IN_STARTUSINGNETWORK

/*
 - WIFI 중 3G 연결시 default router가 2개 잡히는 현상 수정
 - WIFI, Data 설정 상태를 모두 확인 하기 위하여 dhcpclient에서 add route하는 구문을 framework로 이동 (ConnectivityService)  
 - 3G Data중 WIFI 연결시 기존 3G Default Network 삭제
 - default route add/remove function MobileDataStateTracker.java 에서 처리하도록 수정.
 - 3G connected event broadcast 된 직 후config 읽으면 interface up 되지 않는 문제로 broadcast 전에 interface up check. 
   (개인정보 상태에서 초기화 후 마켓 자동 다운로드 되지 않는 문제.)
 - 3G 종료 후에도 config 값 읽으면 한참 후 interface down 으로 변경되어 check 하는 부분 추가. (tethering 에서 문제)
 -[KTF] remove dns route 동작하지 않는 문제로 mIPv4InterfaceName, mIPv6InterfaceName 을 disconnect 에서 null 로 만드는 부분 삭제. 
 - EF45K ICS : ifc_utils.c 에서 property set 후 get 해서 setting 된 상태 체크 부분만 적용됨.
*/
#define FEATURE_SKY_DS_CHANGE_DEFAULT_ROUTE_FOR_MOBILE


/*
 - Fast Dormancy 기능 규현
 - 시나리오 : LCD ON시 네트워크에서 내려오는 메시지로 dormant 진입 판단, LCD OFF시 10초 후 Dormant 진입
 - aArm : LCD ON/OFF 정보를 mArm으로 전달하도록 수정
 - mArm : FEATURE_SKY_DS_FAST_DORMANCY 참고
*/
#define FEATURE_SKY_DS_FAST_DORMANCY

/*
  - PDP Fail 시 loggable fail cause 이면 logging 남길때 단말 죽는 문제로 isEventLoggable() 에서 항상 false 리턴하도록함.
*/
#define FEATURE_SKY_DS_AVOID_FATAL_EXCEPTION

/*
  - 약전계에서 pdp connection fail 후 retry timer 증가하면서 강전계 이동시에 증가된 timer 값으로 retry 하여 연결이 안되는것으로 
    보이는 현상발생하여 screen on 상태에서는 timer 증가시키지 않고, off->on 될 경우 바로 try 하도록수정
*/  
#define FEATURE_SKY_DS_IMMEDIATE_SETUP

/*
  - permanent fail 시 alert pop-up 및 data disable 시킴
   => data enable 상태 유지하고 notification / pop-up 보여줌. 
*/
#define FEATURE_SKY_DS_PDP_REJECT_POPUP

/*
- no service event 가 발생할 경우 실제 data 종료되지 않고 disconnected 로 broadcast 되어 default route 삭제되고 
   바로 in service 오게되면 아래단으로는 data 연결되어 잇지만 상위에는 연결되지 않은 것으로 보이는 문제 
- dataconnectiontracker.java , BSP 별로 주석처리되어 필요 없는 경우 잇음. 
-> ICS 변경후 phonebase.java 에 PROPERTY_OOS_IS_DISCONNECT 로 해당 내용 처리되어 default값을 false 로 변경함. 
*/
#define FEATURE_SKY_DS_NO_SERVICE_BUG_FIX

/*
-  RadioTech 가 unknown 으로 오는 경우 networkInfo의 available 이 false 가 된다.
-  connected 상태에서 이렇게 되면 browser 에서 network 사용할 수 없다는 pop-up 발생되면서 
- 페이지는 로드되는 현상이 발생된다. 
- 수정파일 : MobileDataStateTracker.java    
- networkInfo isAvailable 부분은 KT App 요구사항으로 인해 미적용 하고, CONNECTED 상태 유지상태에서
   UNKNOWN_TECH 올려 Indicator 사라지는 현상만 적용함.  
- 수정파일 : TelephonyRegistry.java
*/
#define FEATURE_SKY_DS_AVOID_UNKNOWN_RADIO_TECH


/*
- ACTION_ANY_DATA_CONNECTION_STATE_CHANGED intent 가 전달되지 않는 경우가 발생하여
   intent broadcast(notifyDataConnection) 후 connectivityservice 에 update 가 되엇는지 체크.
 - dataconnectiontracker.java mmdataconnectiontracker.java
*/
#define FEATURE_SKY_DS_ANY_DATA_INTENT_RECOVERY


/*
- datausage 메뉴에서 background data 차단 시 for loop 를 돌며 UID 별로 ip table 에 set 해 주어 
   app 에서 시간 내 받지 못해 anr 발생 및 background data 차단 설정 시 booting 중 system 에서 
   anr 발생하는 문제 수정   =>각 UID 별로 event 로 처리되도록 함.
- android_filesystem_config.h  에 Define 된 UID 로 system UID 값 변경. 
- system/bin/iptables 는 system 권한으로 되어 잇으나 ip6tables 는 shell 로 되어 잇어 system 으로 변경.
   -NetworkPolicyManagerService.java
*/
#define FEATURE_SKY_DS_BACKGROUND_RESTRICT_BUG_FIX

/*
- system/bin/iptables 는 system 권한으로 되어 잇으나 ip6tables 는 shell 로 되어 잇어 system 으로 변경.
   -android_filesystem_config.h
*/

#define FEATURE_SKY_DS_IP6TABLE_UID_BUG_FIX

/*
- data registration statue query 시 fail 발생하면 data service state 가 no service, radio tech 는 none 으로 처리되는 문제로 
   이전 값을 newSS 에 저장하여 이후 response fail 발생시 이전 값 유지하도록 함. 
- GsmServiceStateTracker.java
*/
#define FEATURE_SKY_DS_DATA_REGISTRATION_QUERY_FAIL_RECOVERY

/*
- MMS send 시 항상 secondary dns address 로 dns query를 보내는 현상에 대한 수정
*/
#define FEATURE_PS_WRITE_PID_DNS_BUF_FIX

#define FEATURE_SKY_DS_IP6TABLE_CMD_BUG_FIX

/************************************************************************************************************************
**    2. Related MENU / UI
************************************************************************************************************************/

/*
 - APN Settings Menu를 Hidden Code(##276#7388464#)로 실행 할 수 있도록 수정
*/
#define FEATURE_SKY_DS_ADD_APN_SETTING_HIDDEN_CODE

/*
 - APN 메뉴 사용자 영역으로 이동 (DS1_DATA_REMOVE_APN_SETTING_MENU 제거)
 - default apn에 대하여 수정 불가 하도록 수정
 - Hidden Code와 일반 사용자 메뉴 구별하여 동작하도록 수정
  - data disable-> enable 시킬 경우 default apn 설정 확인하고, 변경된 경우 reset
*/
#define FEATURE_SKY_DS_PREVENT_EDIT_DEFAULT_APN

/*
- easy setting 기능 추가로, socket data moe 설정하는 부분에서 intent broadcast 해줘야 함.  
- SKT 와 KT 규격 차이로 data mode setting 하는 부분 차이.
*/
#define FEATURE_SKY_DS_EASY_SETTING

/* 
 - hidden menu에서 RRC version 선택에 따라 category 선택이 되도록 수정함.
 - HSUPA category는 NV 처리가 되어 있지 않아서 추가함.
 - R4 only 선택 시 HSDPA category / HSUPA category 항목 disable로 변경함.
 - R5(HSDPA) 선택 시 HSDPA category enable / HSUPA category disable로 변경함.
 - R6(HSDPA) 선택 시 HSDPA category disable / HSUPA category enable로 변경함.
 - HSUPA를 지원하지 않는 칩일 경우, R6 설정 시 폰이 리셋되므로 rrc version 설정에서 R6를 반드시 제거해야 함. 
*/
#define FEATURE_SKY_DS_HSUPA

/* 
 - R7(HSPA) 선택 시 HSDPA category enable / HSUPA category enable 되도록 구현. 
*/
#define FEATURE_SKY_DS_HSPA


/* 
  - TCP Buffer size 수정 
  - system/core/rootdir/nit.rc
*/
#define FEATURE_SKY_DS_LTE_TCP_BUFFER_CHANGE



/************************************************************************************************************************
**     3. CTS / PortBridge / DUN / Log /vpn
************************************************************************************************************************/
#define FEATURE_SKY_DS_ATFWD_PROCESS

/*
- Data Manager 관련 처리. 
- AT$SKYLINK command 로 data manager 실행시키고, 
- /dev/pantech/close file 에 내용이 삭제되면 data manager 가 종료된 것으로 처리한다. 
-/port_bridge/ , skydunservice.java
*/
#define FEATURE_SKY_DS_DATAMANAGER 

/*
 - 전화접속 연결 시 연결/해제 시 toast로 연결/해제 알림
 - notification 영역에 연결 중 상태임을 알림
 - port_bridge에서 SkyDunService.java에서 ui 처리를 하기 위해서 DUN_EVENT_RMNET_DOWN일 경우에도 dun_disable_data_connection()에서 dun file에 DUN_INITIATED를 write 함.
 - port_bridge에서 SkyDunService.java에서 ui 처리를 하기 위해서 DUN_EVENT_RMNET_UP일 경우에도 dun_enable_data_connection()에서 dun file에 DUN_END를 write 함.
 - 3G 접속과 전화접속이 동시에 일어날 경우, DUN_STATE_CONNECTED 상태에서 3G 접속이 먼저 일어날 경우, 전화접속은 연결되지 않고, UI는 연결 중이 된다. 
   이를 해결하기 위해서 DUN_STATE_CONNECTED에서 DUN_EVENT_RMNET_UP 올 경우, 전화접속을 종료한다.
*/
#define FEATURE_SKY_DS_RELATED_DUN_UI

/*
  - USB tethering 에서 wifi 연결 된 경우 wifi 우선 사용하도록 config.xml 및 tethering.java 에 수정 및 추가. 
*/
#define FEATURE_SKY_DS_WIFI_USB_TETHERING

/*
- 특정 VPN 서버 접속 안되는 문제 (참고 : android 는  SSL VPN 과 Cisco VPN 미지원)
- external\mtpd\L2tp.c 및 kernel config ( kernel\arch\arm\config\msm8660-EF34K_deconfig 에서 이미 define된 내용은 주석처리하고 y로 설정 )
*/
#define FEATURE_SKY_DS_VPN_FIX

/*
- CTS testTrafficStatsForLocalhost test 를 위한 kernel config 에 CONFIG_UID_STAT=y 로 수정. 
*/
#define FEATURE_SKY_DS_CTS_LOCALHOST

/*
-  usb tethering/wifi hotspot 시 google dns 서버 사용으로 일부 사이트 연결되지 않는 문제 
- ConnectivityService.java  tethering.java
*/
#define FEATURE_SKY_DS_SET_TETHERED_DNS

/*
테더링 연결 후 WIFI 연결시 3G 아이콘 사라지지 않는 문제
*/
#define FEATURE_SKY_DS_TETHERED_BUG_FIX


#define FEATURE_DS_DEACTIVATE_PENDING_BUG_FIX

/************************************************************************************************************************
**     4. VT
************************************************************************************************************************/
/*
- VT 지원 , CS VT control interface 및 sdio_vt.c 포함.
*/

#define FEATURE_PANTECH_VT_SUPPORT

#define FEATURE_PANTECH_VT_SUPPORT_KT

#define FEATURE_PANTECH_VT_SUPPORT_QMI

#define FEATURE_PANTECH_VT_SUPPORT_QMI_KT

#define FEATURE_PANTECH_VT_SUPPORT_QMI_CALL_TYPE //CSVT call_type 추가 
/*
- Qmi 구조로 변경 된 후에 아래 내용에 대한 적용한 코드가 동작 하지 않아 수정함.
-----------------------아래 ----------------------------------
- NV_NOTACTIVE에 대한 고려가 필요한 항목임.
- NV_ACTIVE 항목은 모뎀에서 Sky_Nv_Protocol에서 초기화가 되지만, 
- NOTACTIVE는 초기화가 되지 않아 READ 할수 없다.  
*/  

#define FEATURE_SKY_DS_GETNV_QVP_DEFAULT


#endif/* T_SKY_MODEL_TARGET_WCDMA */


/* ######################################################################### */


/*****************************************************
    SKT 모델 적용사항
    Feature Name : T_SKY_MODEL_TARGET_SKT
******************************************************/
#ifdef T_SKY_MODEL_TARGET_SKT
#ifdef T_SKY_MODEL_TARGET_KT
#error Occured !!  This section is SKT only !!
#endif

#endif/* T_SKY_MODEL_TARGET_SKT */


/* ######################################################################### */
/*****************************************************
    KT 모델 적용사항
    Feature Name : T_SKY_MODEL_TARGET_KT
******************************************************/
#ifdef T_SKY_MODEL_TARGET_KT
#ifdef T_SKY_MODEL_TARGET_SKT
#error Occured !!  This section is KT only !!
#endif

/************************************************************************************************************************
**    1. Related 3G-connection
************************************************************************************************************************/

/*
 - 3G(GPRS) Data Suspend/Resume 함수
 - GSMPhone의 disableDataConnectivity/enableDataConnectivity는 PLMN 수동 검색시 활용하면 문제가 있어 새로운 함수 추가
 - getDataConnectionState() 에서 connected가 아니면 전부 disconnected 로 return 해서 connecting 상태에서 수동검색 진행되는 문제로 connecting 추가.
*/
#define FEATURE_KT_DS_SUSPEND_RESUME_FUNC

/*
 -KT 3G Data 접속 시나리오 적용 =>> system property 에서 db 의 SOCKET_DATA_CALL_MODE 를 생성해서 적용. kaf oem api 0.9.0 에 default 값을 popup 으로 요구됨.
- setup alert popup 에서 설정 선택시 data mode setup 화면으로 이동.
- data mode setup 화면은 skydatamodesettings.java 로 구현됨. => \packages\apps\Phone\src\com\android\phone\settings.java 로 변경.
=> 2011.10.21 android 제공하는 SOCKET_DATA_CALL_ENABLE 로 변경. 
*/
#define FEATURE_KT_DS_DATA_SETUP

/*
 - data/dun setup 가능한지 체크시 KT PS reject cause 체크 및 toast. 
 - 네트워크 등록중인 경우에는 toast 처리하고 발신시도해야함. skydunservice.java
*/
#define FEATURE_KT_DS_PS_REJECT

/*
 단말 설정 메뉴내 "새 APN"(default.ktfwing.com) 추가 설정, LMS 메시지 발신 후 (WIFI ON/3G ON) APN 확인 시 기본 APN(alwayson-r6.ktfwing.com)을 유지하고 있는 문제 수정.
*/
#define FEATURE_KT_DS_APN_MMS_BUGFIX

/*
 wifi enable 상태에서 wifi disconnect/connect 중 3G 연결 팝업이 자주 발생하여 
 wifi 접속 시간을 고려하여 팝업 발생을 3초 딜레이 시킴. wifi disable 상태는 딜레이 주지 않음.   
*/

#define FEATURE_KT_DS_WIFI_3G_POPUP_DELAY

/*
 wifi on, 3G on 상태에서 mms 전송시 1분후 전송 실패 현상 수정. 
 startusing 사용하는 어플중  MMS 전송시 만기 타이먹 작동하여 1분후 종료하는 부분 막음. 
*/
#define FEATURE_KT_DS_DISABLE_TIMER_IN_MMS

/*
- data mode on/off 시 lte 망 attach,detech 하도록 pref mode 변경내용 추가.
- mmdataconnectiontracker.java
*/
#define FEATURE_KT_DS_LTE_DATA_SETUP

/*
- KAF Req. startUsingNetworkFeature() 에 feature 를 "KT_Internet" 으로 요청해 data 연결 설정 및 상태를 가져간다. 
- ConnectivityService.java
*/
#define FEATURE_KT_DS_KAF_CONNECTIVITY

/* KT 요구 사항에 의 거하여 MSS size 1410 변경( MTU1450으로 변경 ) */
#define FEATURE_KT_DS_CHANGE_MTU

#define FEATURE_SKY_DS_CNE_DISABLE 
/*
 - CS Call 중에는 PS Data를 Block하여 Call Drop을 방지 한다.
*/
#define FEATURE_KT_DS_VOICE_CALL_PROTECTION_IN_MULTI_RAB

/*
- Data mode disable 인 경우 mms network info 에 isAvailable 을 false 로 return 
- MMS 요청사항. 
*/
#define FEATURE_KT_DS_MMS_UNAVAILABLE_ON_DATA_DISABLE


#define FEATURE_SKY_DS_MULTIPLE_PARTIAL_RETRY_BLOCKING

/*
- ISkyDataConnection에 3rd parth app에서 데이터 사용량 초과 제한을 판단할 수 있는 API 추가
*/
#define FEATURE_SKY_DS_NET_OVERLIMIT_API

/************************************************************************************************************************
**    2. Related MENU / UI
************************************************************************************************************************/
/*
  packages\apps\Phone\src\com\android\phone\settings.java 및 network_settings.xml 에서 처리하도록 변경됨. 
=>   2011.10.21 MobileNetworkSettings.java / mobile_network_settings.xml 로 변경됨. 
*/
#define FEATURE_KT_DS_ADD_ALWAYSON_MENU


/*
  - Lab Test Menu에 vt emulator 메뉴 추가하여 ext 324 NV 설정기능 추가
*/
#define FEATURE_KT_DS_VT_EMULATOR

/*
- lock 상태에서 incoming call 수신된 상태에서 wifi 연결 종료되어 data pop 표시되고 설정 선택하면 lock 화면 보이는 문제.
- telephonyintent 에 ringcall start/end 추가하여 broadcast 하면 data popup에서 ringing 이면 설정메뉴 진입못하게함. 
- GsmCallTracker.java, TelephonyIntents.java, MMDataConnectionTracker.java
=> 2011.10. KAF 검수 절차서 참고, 시나리오변경. pop-up 상태에서 incoming call 발생하면 pop-up 닫고 data 연결. 
*/
#define FEATURE_KT_DS_POPUP_IN_RINGSCREEN_BUG

/* 
- Roaming 시 data roaming enable uncheck 상태이면 notification 영역에 보여주고 지워지지 않도록 한다.
- 설명이 두줄 로 보이도록 함, 선택시 설정 메뉴로 이동. 
*/
#define FEATURE_KT_DS_ROAMING_SETUP


/************************************************************************************************************************
**     3. CTS / PortBridge / DUN / Log
************************************************************************************************************************/
/*
: KT OTA command 처리 "AT*KTF*OPENING" ./port_bridge/ , skydunservice.java
*/
#define FEATURE_KT_DS_EIF_OTA 

#define FEATURE_KT_DS_ATFWD_COMMAND

/*
  - dun 연결상태를 mmdataconnectiontracker.java 에서 저장하고 state 를 return 한다.
  => sky feature 에서 KT feature 로 변경. 
*/
#define FEATURE_KT_DS_DUN_SERVICE


/************************************************************************************************************************
**     4. VT
************************************************************************************************************************/

/*
- KT 영상전화 단말 규격 disconnect cause 추가, qcril 에서만 undefine 해 주면 된다. 
- local ringback tone 관련 call progress info(#1,#2,#3,#8) 체크 값 추가. 
*/
#define FEATURE_PANTECH_VT_SUPPORT_KT

#endif/* T_SKY_MODEL_TARGET_KT */


/************************************************************************************************************************
** 삭제된 feature
************************************************************************************************************************/

/*  
#define FEATURE_SKY_DS_PRESERVATION_WAKEUP_BUG_FIX
=> FEATURE_SKY_DS_NO_SERVICE_BUG_FIX 로 변경. 
- Preservation 상태에서  RA update 중에 rrc abort 로 limited event 수신 시 이후 Preservation 벗나어지 못하는 문제 수정 
*/

/*  
#define FEATURE_SKY_DS_CNE_DISABLE 
 => CNE 삭제됨. 
 - 2030 버전에서 CNE enable 되어 WIFI연결시 3G 종료되지 않는 문제점 발생.
 - CNE 부분 임시로 주석 처리. 
*/

/*
#define  FEATURE_SKY_DS_STABILITY_TEST
 =>  cust_sky.h 에 FEATURE_PANTECH_STABILITY 로 featuring 됨. 
*/

/*
#defien FEATURE_SKY_DS_ADD_DATA_LOG
- framework 단 로그 추가 내용, 삭제함. 
*/

/*
#define FEATURE_SKY_DS_CHANGE_MASTER_VALUE_TURE
 - mMasterDataEnabled이 false가 되어 Data호를 시도 하지 못하는 현상 수정
 - 재현이 어렵고 mMasterDataEnabled의 경우 eclair 이후 그 존재 의미가 없어 Data호 시도시 해당 Value 체크 구문 삭제 
   =>3G 설정 값이나 로밍 설정 값을 반영하려고 했으나, 차단시 MMS 발신 때문에 무조건 true로 리턴하도록 수정.
*/

/*
#define FEATURE_SKY_DS_CSS_INDICATOR_BUG_FIX
- EF34K 1085 patch 로 css indicator 처리 삭제됨. 
- CSS Indicator 값이 0 으로 전달되어 
- DataServiceStateTracker.java 에 포함되어 있는데 내용확인 필요.

*/

/*
#define FEATURE_SKY_DS_IDLE_PDL
==> System team 에서 처리하기로 함. 
- AT*PHONEINFO command 에 PDL command 실행시킴. system servier로 command 전달.
 -idle download support
*/

/*
#define FEATURE_KT_DS_DUN_BLOCKING_IN_CS_ONLY
  - CS only 인 경우 DUN 연결 시도하지 못하도록 함.
  - mmdataconnectiontracker.java 에서 ps restricted 인 경우 
  gsm.net.psrestricted property 에 1을 써주고 skydunservice 에서 이 값을 체크한다.
*/

/*
#define USB_TETHERING_ERROR_TEMP
- nativeDaemonConnector.java 에 USB Tetehring 관련 수정 사항 1087 이후 수정된 사항으로 temp 처리 ....
*/

/*
#define FEATURE_SKY_DS_END_DUN
 - 전화접속을 UI에서 끊을 경우, RPC를 이용해 전화접속이 끊어지도록 구현
 - 3G 연결 설정을 해제 할 경우 사용함.
 - 향후, notification 영역에서 전화접속을 선택할 경우 종료할 수 있는 팝업 제공하여 UI에서 종료할 경우에도 사용함.
 - framework 구조 상이해서 mmdataconnectiontracker 에서 commandsinterface api call 하도록 변경됨.
=> 2011.10.21 KT 사용하지 않음. 
*/

/*
#define FEATURE_SKY_DS_DUN_TEST_MENU
 - Froyo 버전에서 Tethering 기능이 추가되어 전화 접속은 nettest 메뉴에서 설정해야만 연결할 수 있도록 구현함.
 - \LINUX\android\pantech\apps\NetTest\src\com\pantech\app\nettest\DunTest.java 추가 
 - \LINUX\android\pantech\apps\NetTest\res\layout\duntest.xml 추가 
 - [kt] 전화 접속 테더링 동시 지원으로 삭제 -  android\pantech\apps\nettest\AndroidManifest.xml 에서만 주석 처리 
=>2011.10.21 KT 사용하지 않음
*/

/*
#define FEATURE_SKY_DS_DUN_USER_MODE 
 - 문제점 : QualcommSetting 디렉토리에 있는 파일들은 user mode 시 실행되지 않으므로, dun, sync manager, data manger, curi explore가 실행되지 않음.
 - DunService.java 대신 LINUX\android\packages\apps\Phone\src\com\android\phone\SkyDunService.java를 추가함.
 - SkyPhoneBroadcastReceiver.java 에서 dun service start 하고, Dun_Autoboot.java에서는 서비스 start 하는 부분을 막음.
 - QualcommSetting\AndroidManifest.xml에서 dun service를 제거하고, packages\apps\Phone\AndroidManifest.xml에 sky dun service 추가
=> 2011.10.21 atfwd 처리로 변경됨. 
*/

/*
#define FEATURE_SKY_DS_PORTBRIDGE_EXTERNAL
 - port_bridge(dun 관련) 처리 공통 feature. ./port_bridge/ , skydunservice.java 
 - 초기 부팅시 dun_ext_smd_ctrl에서 TIOCMGET에 대한 정보가 없으면 dun_monitor_ports스레드가 시작되지 않음. (TIOCMGE정보 보다 스레드가 먼저 생성) 
 - dun_port_dataxfr_up 스레드가 멈추지 않고 돌면서 CPU의 대부분을 점유함.
 - MODEM의 TIOCMGET정보가 생성되기 전에 dun_monitor_ports 스레드가 생성 되면 다시 스레드가 생성 되게 수정.
 => 2011.10.21 atfwd 에서 처리로 변경됨.
*/

/*
#define FEATURE_SKY_DS_JAVA_CACHE
 - 3G OFF 상태에서 MMS 전송시 DNS 쿼리  IP add  결과가 NULL로 넘어 오는 현상 발생. 
 - unknownhost일 경우 cache 에 저장되지 않도록 주석 처리. 
*/

/*
#define FEATURE_KT_DS_SW_RESET_RELEASE_MODE_NO_DATA_POPUP

 - 단말 리셋 발생시 3G 연결 팝업(부팅시 팝업 ) 발생하지 않도록 수정. 
 mmdataconnectiontracker.java   SurfaceFlinger.cpp
*/

/*
- Mobile data 연결 후 default route add 시 fail 발생하면 ip 로 다시 add 해주도록. 
- networkStateTracker.java에서 teardown() 하고 바로 reconnect()하게되면 disconnect 되지 않아. IP 로 add 해줌.
- 수정파일 : NetworkStateTracker.java 

#define FEATURE_SKY_DS_DEFAULT_ROUTE_RESTORE
*/

/*
- radio off event 발생할 경우(rild dead or lpm or rpc reset..etc) data disconnect 시켜서 radio on 될 경우 정상동작 하도록 보완코드 추가.
- mmdataconnectiontracker.java onRadioOff()
- FEATURE_KT_DS_DISCONNECT_CHECK 에서 공통 feature 로 변경. 
- EF45K ICS  수정되어 feature 삭제함. 
#define FEATURE_SKY_DS_DISCONNECT_CHECK
*/

/* 
 - TIMEOUT_INITIAL을 1초로 줄임.
 - dhcpclient.c  Wi-Fi 수정내용으로 Wi-Fi 로 이관.
#define FEATURE_SKY_DS_DHCP_DISCOVER_TIMER
*/

/*
- GB version에서 startUsingNetworkFeature 에서 이미 connected 된 service type인 경우 
- dns configuration change 처리에 network type 을 사용하여 항상 mobile 로 set 해서
- private dns list 에 pid 정상등록되지 않는 문제 
- connectivityService.java
- GB 버전 업 이후 해당내용 수정되어 feature 삭제.
#define FEATURE_SKY_DS_DNS_CONFIG_BUG_FIX
*/

/*
- data disconnected 된 순간 _test_dns_check 수행해서 mms전송하려고 하면 inetaddress.getbyname() 하면 unknown host exception 발생
- getaddrinfo.c ( LINUX\android\bionic\lib\netbsd\net\)
- IPv6 로 dns query 시도하는 문제로 feature 삭제함. 
#define FEATURE_SKY_DS_DISABLE_TEST_DNS
*/

/*
- data connecting 상태에서 disable 시키면 service type disable 시키지 않는 문제 
- DataConnectionTracker.java
- GB 버전 업 이후 해당내용 수정되어 feature 삭제.

#define FEATURE_SKY_DS_DISABLE_SERVICE_TYPE_BUG_FIX
*/

/*
- tether setting 에서 oncreate 시 초기값 update   
  tethersettings.java
  설정메뉴 R&R 변경됨. 
#define FEATURE_SKY_DS_TETHER_STATE_INIT
*/

/*
 - 테더링 관련 도움말에 나와 있는 URL이 Nexus One 내용이므로 해당 내용 삭제(상기협의)
 - Wifi hotspot에 대한 도움말이 Wifi 테터링으로 설명되어 있어 수정함.(한글 수정, QE 문제점)
 - 20120227 확인 결과 현재 구글 내용으로 변경되어 해당 피쳐 삭제함.
#define FEATURE_SKY_DS_TETHERING_HELP
*/

/*
 - overylay 폴더에 framework단 resource 등록 
 - 사용하지 않음.
#define FEATURE_SKY_DS_RESOURCE 
*/

/* 
- Data 연결 ICON default 값을 3G 로 설정하여 G 아이콘이 보이지 않도록 수정. 
- 20120227 소스 변경으로 피쳐 적용 안함.
#define FEATURE_SKY_DS_DEFAULT_DATA_ICON
*/

/*
- Data Setup Fail 발생 후 WAITING_ALARM state 에서 reconnect alarm intent 에서 scanning 상태로 set 해서 
- data disable 상태에서 connecting 으로 state 올라가는 문제 수정. 
- MMDataConnectionTracker.java
- 20120227 소스 변경으로 피쳐 적용 안함.
#define FEATURE_SKY_DS_STATE_BUG_FIX
*/

/*
- Roaming 시 intent "ACTION_ANY_DATA_CONNECTION_STATE_CHANGED"가 전달 되지 않아 
로밍으로 인한 재 부팅 후 notification이 발생하지 않는 문제 수정.
- 20120227 MMDataConnectionTracker.java 부재로 인해 적용 안함. 
#define FEATURE_KT_DS_ROAMING_SETUP_BUG_FIX
*/

/*
 - EF18K IOT 1차 LMS 1, SBSM 8 요청 사항에 대한 MMS TestMenu 추가 요청에 대해 ##46632874# > 엔지니어모드에 추가 함.
 - \LINUX\android\packages\apps\Settings\src\com\android\settings\skyhiddenmenu\KtHiddenMenu.java
 - \LINUX\android\packages\apps\Settings\res\xml\kt_hidden_menu.xml
 - \LINUX\android\packages\apps\Settings\res\values\strings_cp.xml
 - 20120227EF45K 부터 OMA MMS 적용에 따라 삭제.
#define FEATURE_KT_DS_MMS_TESTMENU 
*/



/*
- DNS/gatway 주소를 체크하여 없으면 dataconnection을 teardown 시킴.
=> EF45K ICS 에 code 수정됨
#define FEATURE_SKY_DS_BAD_IP_CHECK
*/

/*
- tearDwonData()의 파라미터가 잘못 되어 수정함. 
- EF45K ICS 에 코드 수정됨.
#define FEATURE_SKY_DS_PARAMS_BUG_FIX
*/

/*
 - 모바일 네트워크 설정 메뉴에 데이터 사용 메뉴 삭제 
 => EF45K ICS 에서 data usage 변경되엇으며, 담당팀 변경됨.
#define FEATURE_SKY_DS_DATA_USAGE_MENU_BLOCKING
*/

/*
 - network interface, APN, socket, Concurrent(3G/WIFI) 관련 테스트 APP 추가
 - WIFI Debug Screen 추가
 - android/pantech/apps/skyLabTest 폴더 
 - 피쳐는 적용하지 않고 apk로 소유함.
#define FEATURE_SKY_DS_ADD_NETTEST
*/

/*
- 로밍 설정 시 주의 팝업 다이얼로그 에서 홈 이동 후 재진입시 onResume 에서 check box update 하여 이후 체크 박스에서 
  선택하여도 다이얼로그 보여지지 않는 현상 수정 
#define FEATURE_SKY_DS_FIX_ROAM_CHECK_UI_BUG 
*/
/*
  - root process issue 로 devices.c, init.rc, init.qcom.rc 에서 수정된 내용. 
  - cnd, netmgrd, port-bridge, 브ㅕㅌㅇ
  - port_bridge 의 경우 수정된 code 사용되어야 한다.
  - init.rc 혹은 init_모델명.rc 파일에서 /dev/pantech directory 를 system 권한으로 생성해줘야한다. 
#define FEATURE_SKY_DS_CTS_ROOT_PROCESS
*/

/*
- qualcomm orignal code중에 사용하지 않는 부분 막음 
#define FEATURE_SKY_DS_NOT_USE_QUALCOMM_ORIGINAL
*/

/*
- vpn setting 메뉴 bug fix.
#define FEATURE_SKY_DS_VPN_SETTING_BUG
*/

/*
- SUID file finder 에서 IP 가 SUDI 로 check되어 fail되는 현상
  80-N6956-2_A 문서 참고
#define FEATURE_SKY_DS_CTS_SUID_CHECK
*/

/*
 - KTF 요구사항 : 3G Data가 Disable 되어도 MMS 송/수신은 정상 동작 해야 함. 
 - Data Diable로 설정되어 있어도 MMS 수신시 PDP를 올릴수 있도록 수정
 - KT 요구사항 변경 : DATA Disable 인 경우 MMS 송수신 불가함. WIFI /DUN연결인 경우만 feature 부분 사용. 
#define FEATURE_KT_DS_ALLOW_MMS_IN_DATA_DISABLE
*/

/*
 - IPv4 연결상태에서 IPv6 max fail 된 경우 data connection fail 을 noti 하여 connectivity쪽에 data fail 로 전달되는 문제 
 - mmdataconnectiontracker.java
#define FEATURE_KT_DATA_CONNECTION_FAIL_BUG_FIX
 */


#endif /* __CUST_SKY_DS_H__ */
