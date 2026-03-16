#ifndef PROTOCOLTYPES_H
#define PROTOCOLTYPES_H

#include <QtGlobal>
#include <QByteArray>
#include <QVector>

// ==================== 通用前缀 ====================
const quint8 PREFIX_UP_SEND   = 0xA0 ;   // 上位机发送大多以0xA开头
const quint8 PREFIX_DOWN_RESP = 0xD0 ;   // 下位机响应大多以0xD开头
#pragma pack(push, 1)   // 严格1字节对齐，与协议完全一致
// ==================== 命令码枚举 ====================
enum class CmdCode : quint8 {
    IO_Init               = 0x34,   // 0xA4 0x34
    Voltage_Reg_IO        = 0x31,   // 0xA1 0x31
    Voltage_Reg_DA        = 0x32,   // 0xA1 0x32
    Sample_Control        = 0x33,   // 0xA3 0x33
    Read_Log              = 0x31,   // 0xAF 0x31
    Analog_Output         = 0x34,   // 0xA5 0x34
    Comm_Status_Random    = 0x00,   // 随机码相关
    // ... 可继续添加
};

// ==================== IO初始化 ====================
struct IOInitRequest {
    quint8  prefix = 0xA4;
    quint8  cmd    = 0x34;
    quint32 ioFunc[2];          // IO功能定义（输入/输出）
    quint32 ioLevel[2];         // 输出初始电平
    quint8  encoderSlave = 0;   // 0:不存在 1:存在
    quint8  reserved[13] = {0xDD}; // 填充0xDD
};

// 下位机响应
struct IOInitResponse {
    quint8 prefix = 0xD4;
    quint8 success;             // 0成功 1长度错误
    quint8 reserved[14] = {0xDD};
};

// ==================== 调压指令 ====================
struct VoltageRegRequest {
    quint8  prefix = 0xA1;
    quint8  cmd;                // 0x31 IO调压 / 0x32 DA调压
    quint8  upIO;               // 升压IO号
    quint8  upLevel;            // 升压使能电平
    quint8  downIO;
    quint8  downLevel;
    quint8  adChannel;          // 模拟量采集通道
    float   ratio;              // 电压转换比率
    float   targetVoltage;
    float   errorRange;
    quint8  timeoutSec;         // 调压超时时间（秒）
    quint8  daChannel;          // DA调压通道（仅DA模式有效）
    quint8  powerType;          // 0直流 1交流
    quint8  reserved[11] = {0xDD};
};

// 下位机立即响应（16字节）
struct VoltageRegAck {
    quint8 prefix = 0xD1;
    quint8 success;             // 0成功 1长度错 2未初始化
    quint8 reserved[14] = {0xDD};
};

// 下位机调压结果返回
struct VoltageRegResult {
    quint8 prefix = 0xD2;
    quint8 errorCode;           // 0正常 1超时 2电源损坏...
    float  realVoltage;         // Little-Endian
    quint8 reserved[10] = {0xDD};
};

// ==================== 采样与控制指令（最复杂） ====================
struct SampleControlRequestHeader {
    quint8  prefix = 0xA3;
    quint8  cmd = 0x33;
    quint8  isLastPacket;
    quint32 packetSeq;
    quint8  adChipMask;         // 每位表示一个AD芯片
    quint32 sampleRate;
    quint32 sampleTimeMs;
    quint8  digitalChannelMask; // 数字量通道
    quint8  enableEncoder;
    quint16 ioCmdLength;        // 0~1005 或 0xFFFF（自主判断模式）
    // 后面跟 IoCommands 字符串或自主判断结构体
};

// 自主判断模式参数（当 ioCmdLength == 0xFFFF 时使用）
struct AutonomousParam {
    quint8  startWithTripOrClose;   // 0分 1合
    quint8  actionCountN;
    quint32 coilTimeoutMs;
    quint32 closeHoldMs;
    quint32 tripHoldMs;
    quint32 rejectCloseMs;
    quint32 rejectTripMs;
    quint8  controlType;            // 0xB0内控 0xB1外控 0xB2手力
    quint8  tripIO;
    quint8  closeIO;
    quint8  tripCoilStatusIO;
    quint8  closeCoilStatusIO;
    quint8  contactSignalCount;     // 1~3
    quint8  contactIO[3];
    // 后面跟 N个 uint32 动作间隔时间
};

// ==================== 采样数据基本包（72字节） ====================
struct SampleBasicPacket {
    quint8  prefix = 0xB0;
    quint8  errorCode;
    quint32 actualSampleRate;
    quint32 pointsPerChannel[12];   // 每路总点数（编码器等）
    quint32 encoderBytes[12];       // 各编码器通道字节数
    double  calibrationFactor;
    quint8  errorActionIndex;       // 自主判断模式下错误发生在第几次动作
    quint8  rejectDetail;           // 拒分拒合详情（位字段）
    quint8  reserved[5] = {0xDD};
};

// ==================== 采样数据块信息包 ====================
struct SampleBlockInfo {
    quint8  prefix = 0xB8;
    quint16 blockSeq;
    quint16 packetCountInBlock;
    quint8  reserved[11] = {0xDD};
};

// ==================== 采样数据包 ====================
struct SampleDataPacketHeader {
    quint8  prefix = 0xB1;
    quint8  channel;                // 1~40
    quint32 packetSeq;
    quint16 validDataBytes;         // 本包有效字节数（≤1392）
    // 后面跟 validDataBytes 个采样数据（2字节/点）
};

// ==================== 随机码 / 普通码联机 ====================
struct CommStatusPacket {
    quint8  prefix = 0xA5;
    quint8  status;                 // 0查询随机码 2下发随机码 3查询普通码
    quint16 randomCode;
    quint8  reserved[12] = {0xDD};
};

#pragma pack(pop)
#endif // PROTOCOLTYPES_H
