#ifndef IEC_GLOBAL_H
#define IEC_GLOBAL_H


#include <QObject>

const int NoDataLens = 13;
const int UIFrameLens = 6;
const int LensOfAPDU = 15;

enum E_FRAME_TYPE
{
    FRAME_TYPE_U,
    FRAME_TYPE_I,
    FRAME_TYPE_S,
    FRAME_TYPE_UNKNOW
};

enum E_UFRAME_TYPE{
    UFRAME_TYPE_STARTFR_C,
    UFRAME_TYPE_STARTFR_A,
    UFRAME_TYPE_STOPDT_C,
    UFRAME_TYPE_STOPDT_A,
    UFRAME_TYPE_TESTFR_C,
    UFRAME_TYPE_TESTFR_A
};

enum E_IFRAME_TYPE{
    IFRAME_TYPE_SINGLEYX,
    IFRAME_TYPE_DOUBLEYX,
    IFRAME_TYPE_SINGLEYK,
    IFRAME_TYPE_DOUBLEYK,
    IFRAME_TYPE_YC
};

const quint16 
    TYPE_REASON_UNUSE = 0x00,
    TYPE_REASON_FOR = 0x01,
    TYPE_REASON_SCALE = 0x02,
    TYPE_REASON_ACCEDENT = 0x03,
    TYPE_REASON_INIT_DONE = 0x04,
    TYPE_REASON_REQUEST = 0x05,
    TYPE_REASON_ACTION = 0x06,
    TYPE_REASON_ACTION_A = 0x07,
    TYPE_REASON_ACTIONSTOP = 0x08,
    TYPE_REASON_ACTIONSTOP_A = 0x09,
    TYPE_REASON_ACTIONEND = 0x0a,
    TYPE_REASON_FILETRANS = 0x0d,
    TYPE_REASON_CALL_ALL = 0x14;


const quint8
    TYPE_LEIXING_SYX = 0x01,
    TYPE_LEIXING_DYX = 0x03,
    TYPE_LEIXING_CLZGYYC = 0x09,
    TYPE_LEIXING_CLZBDYC = 0x0B,
    TYPE_LEIXING_CLZDFDYC = 0x0D,
    TYPE_LEIXING_CP56SYX = 0x1E,
    TYPE_LEIXING_CP56DYX = 0x1F,
    TYPE_LEIXING_SYK = 0x2D,
    TYPE_LEIXING_DYK = 0x2E,
    TYPE_LEIXING_CALLCMD = 0x64;



struct APDU_U {
    quint8 head;
    quint8 lens;

    quint8 res : 2,
        startfr_c : 1,
        startfr_a : 1,
        stopdt_c : 1,
        stopdt_a : 1,
        testfr_c : 1,
        testfr_a : 1;

    quint8 ctrl2;
    quint8 ctrl3;
    quint8 ctrl4;

    APDU_U() {
        head = 0x68;
        lens = 0x04;
        res = 0x03;

        startfr_c = startfr_a = 0x00;
        stopdt_c = stopdt_a = 0x00;
        testfr_c = testfr_a = 0x00;

        ctrl2 = ctrl3 = ctrl4 = 0x00;
    }
};

struct APDU_S
{
    quint8 head;
    quint8 lens;

    quint8 res1 : 1,
        ctrl1 : 7;
    quint8 ctrl2;
    union
    {
        uint16_t recv;
        struct S_recv {
            quint8 res2 : 1,
                ctrl3 : 7;
            quint8 ctrl4;
            S_recv()
            {
                res2 = 0x00;
            }
        };
    };

    APDU_S()
    {
        head = 0x68;
        lens = 0x04;
        res1 = 0x01;
        ctrl1 = ctrl2 = 0x00;
    }
};

struct SINGLEYX // 单点遥信
{
    union {
        quint8 SINYXCMD;
        struct singleYX {
            quint8 data : 1, // 开关分合
                res : 3, // 填充
                block : 1, // 封锁
                replace : 1, // 取代
                nowvalue : 1, // 当前值
                valid : 1;  // 合法

            singleYX()
            {
                res = block = replace = nowvalue = valid = 0;
            }
        }_sYx;
    };
    SINGLEYX()
    {
    }
};

struct DOUYX // 双点遥信
{
    union {
        quint8 DOUYXCMD;
        struct doubleYX {
            quint8 data : 2, // 开关分合
                res : 2, // 填充
                block : 1, // 封锁
                replace : 1, // 取代
                nowvalue : 1, // 当前值
                valid : 1;  // 合法

            doubleYX()
            {
                res = block = replace = nowvalue = valid = 0;
            }
        }_dYx;
    };
};

struct SINGLEYK // 单点遥控
{
    union {
        quint8 SINYKCMD;
        struct singleYK {
            quint8 data : 1, // 开关分合
                res : 6, // 填充
                exe : 1;  // 执行
        };
    };
};

struct DOUYK // 双点遥控
{
    union {
        quint8 DOUYKCMD;
        struct doubleYK {
            quint8 data : 2, // 开关分合
                res : 5, // 填充
                exe : 1;  // 执行
        };
    };
};

struct YC // 遥测
{
    union {
        uint16_t data; // 遥测值
        struct d {
            quint8 d1 : 8,
                d2 : 8;
        }origin;
    };
    quint8 ms; // 品质描述
};

struct ASDU {
    quint8 type; // 类型

    union {
        quint8 kb;
        struct _ {
            quint8 number : 7, // 数量（可变结构限定词）
                sq : 1; // 地址是否连续 （可变结构限定词）
        }_kb;
    };

    union {
        uint16_t reason;
        struct _ {
            uint16_t reason : 14, // 传输原因
                pOrN : 1, // 肯定确认or否定确认
                T : 1; // 未试验or试验
        }_reason;
    };

    uint16_t tuAdr; // 装置地址

    quint8 infoAdr1; // 信息体地址
    quint8 infoAdr2;
    quint8 infoAdr3;
};

struct APDU_I // I帧格式
{
    uint8_t head;
    uint8_t lens;

    union
    {
        uint16_t send;
        struct S_send {
            uint8_t res1 : 1,
                ctrl1 : 7;
            uint8_t ctrl2;
        }_send;
    };
    union
    {
        uint16_t recv;
        struct S_recv {
            uint8_t res2 : 1,
                ctrl3 : 7;
            uint8_t ctrl4;
        }_recv;
    };

    ASDU asdu;

    APDU_I()
    {
        head = 0x68;
    }
};

struct cp56time2a // 时标
{
    uint16_t msec;
    uint8_t min : 6,
        res1 : 2;
    uint8_t hour : 5,
        res2 : 3;
    uint8_t day : 5,
        week : 3;
    uint8_t month : 4,
        res3 : 4;
    uint8_t year : 7,
        res4 : 1;
};

#endif // IEC_GLOBAL_H
